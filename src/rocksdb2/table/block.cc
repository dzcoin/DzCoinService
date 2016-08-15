﻿//  copyright (c) 2013, facebook, inc.  all rights reserved.
//  this source code is licensed under the bsd-style license found in the
//  license file in the root directory of this source tree. an additional grant
//  of patent rights can be found in the patents file in the same directory.
//
// copyright (c) 2011 the leveldb authors. all rights reserved.
// use of this source code is governed by a bsd-style license that can be
// found in the license file. see the authors file for names of contributors.
//
// decodes the blocks generated by block_builder.cc.

#include "table/block.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "rocksdb/comparator.h"
#include "table/format.h"
#include "table/block_hash_index.h"
#include "table/block_prefix_index.h"
#include "util/coding.h"
#include "util/logging.h"

namespace rocksdb {

// helper routine: decode the next block entry starting at "p",
// storing the number of shared key bytes, non_shared key bytes,
// and the length of the value in "*shared", "*non_shared", and
// "*value_length", respectively.  will not derefence past "limit".
//
// if any errors are detected, returns nullptr.  otherwise, returns a
// pointer to the key delta (just past the three decoded values).
static inline const char* decodeentry(const char* p, const char* limit,
                                      uint32_t* shared,
                                      uint32_t* non_shared,
                                      uint32_t* value_length) {
  if (limit - p < 3) return nullptr;
  *shared = reinterpret_cast<const unsigned char*>(p)[0];
  *non_shared = reinterpret_cast<const unsigned char*>(p)[1];
  *value_length = reinterpret_cast<const unsigned char*>(p)[2];
  if ((*shared | *non_shared | *value_length) < 128) {
    // fast path: all three values are encoded in one byte each
    p += 3;
  } else {
    if ((p = getvarint32ptr(p, limit, shared)) == nullptr) return nullptr;
    if ((p = getvarint32ptr(p, limit, non_shared)) == nullptr) return nullptr;
    if ((p = getvarint32ptr(p, limit, value_length)) == nullptr) return nullptr;
  }

  if (static_cast<uint32_t>(limit - p) < (*non_shared + *value_length)) {
    return nullptr;
  }
  return p;
}

void blockiter::next() {
  assert(valid());
  parsenextkey();
}

void blockiter::prev() {
  assert(valid());

  // scan backwards to a restart point before current_
  const uint32_t original = current_;
  while (getrestartpoint(restart_index_) >= original) {
    if (restart_index_ == 0) {
      // no more entries
      current_ = restarts_;
      restart_index_ = num_restarts_;
      return;
    }
    restart_index_--;
  }

  seektorestartpoint(restart_index_);
  do {
    // loop until end of current entry hits the start of original entry
  } while (parsenextkey() && nextentryoffset() < original);
}

void blockiter::seek(const slice& target) {
  if (data_ == nullptr) {  // not init yet
    return;
  }
  uint32_t index = 0;
  bool ok = false;
  if (prefix_index_) {
    ok = prefixseek(target, &index);
  } else {
    ok = hash_index_ ? hashseek(target, &index)
      : binaryseek(target, 0, num_restarts_ - 1, &index);
  }

  if (!ok) {
    return;
  }
  seektorestartpoint(index);
  // linear search (within restart block) for first key >= target

  while (true) {
    if (!parsenextkey() || compare(key_.getkey(), target) >= 0) {
      return;
    }
  }
}

void blockiter::seektofirst() {
  if (data_ == nullptr) {  // not init yet
    return;
  }
  seektorestartpoint(0);
  parsenextkey();
}

void blockiter::seektolast() {
  if (data_ == nullptr) {  // not init yet
    return;
  }
  seektorestartpoint(num_restarts_ - 1);
  while (parsenextkey() && nextentryoffset() < restarts_) {
    // keep skipping
  }
}

void blockiter::corruptionerror() {
  current_ = restarts_;
  restart_index_ = num_restarts_;
  status_ = status::corruption("bad entry in block");
  key_.clear();
  value_.clear();
}

bool blockiter::parsenextkey() {
    current_ = nextentryoffset();
    const char* p = data_ + current_;
    const char* limit = data_ + restarts_;  // restarts come right after data
    if (p >= limit) {
      // no more entries to return.  mark as invalid.
      current_ = restarts_;
      restart_index_ = num_restarts_;
      return false;
    }

    // decode next entry
    uint32_t shared, non_shared, value_length;
    p = decodeentry(p, limit, &shared, &non_shared, &value_length);
    if (p == nullptr || key_.size() < shared) {
      corruptionerror();
      return false;
    } else {
      key_.trimappend(shared, p, non_shared);
      value_ = slice(p + non_shared, value_length);
      while (restart_index_ + 1 < num_restarts_ &&
             getrestartpoint(restart_index_ + 1) < current_) {
        ++restart_index_;
      }
      return true;
    }
  }

// binary search in restart array to find the first restart point
// with a key >= target (todo: this comment is inaccurate)
bool blockiter::binaryseek(const slice& target, uint32_t left, uint32_t right,
                  uint32_t* index) {
  assert(left <= right);

  while (left < right) {
    uint32_t mid = (left + right + 1) / 2;
    uint32_t region_offset = getrestartpoint(mid);
    uint32_t shared, non_shared, value_length;
    const char* key_ptr =
        decodeentry(data_ + region_offset, data_ + restarts_, &shared,
                    &non_shared, &value_length);
    if (key_ptr == nullptr || (shared != 0)) {
      corruptionerror();
      return false;
    }
    slice mid_key(key_ptr, non_shared);
    int cmp = compare(mid_key, target);
    if (cmp < 0) {
      // key at "mid" is smaller than "target". therefore all
      // blocks before "mid" are uninteresting.
      left = mid;
    } else if (cmp > 0) {
      // key at "mid" is >= "target". therefore all blocks at or
      // after "mid" are uninteresting.
      right = mid - 1;
    } else {
      left = right = mid;
    }
  }

  *index = left;
  return true;
}

// compare target key and the block key of the block of `block_index`.
// return -1 if error.
int blockiter::compareblockkey(uint32_t block_index, const slice& target) {
  uint32_t region_offset = getrestartpoint(block_index);
  uint32_t shared, non_shared, value_length;
  const char* key_ptr = decodeentry(data_ + region_offset, data_ + restarts_,
                                    &shared, &non_shared, &value_length);
  if (key_ptr == nullptr || (shared != 0)) {
    corruptionerror();
    return 1;  // return target is smaller
  }
  slice block_key(key_ptr, non_shared);
  return compare(block_key, target);
}

// binary search in block_ids to find the first block
// with a key >= target
bool blockiter::binaryblockindexseek(const slice& target, uint32_t* block_ids,
                          uint32_t left, uint32_t right,
                          uint32_t* index) {
  assert(left <= right);
  uint32_t left_bound = left;

  while (left <= right) {
    uint32_t mid = (left + right) / 2;

    int cmp = compareblockkey(block_ids[mid], target);
    if (!status_.ok()) {
      return false;
    }
    if (cmp < 0) {
      // key at "target" is larger than "mid". therefore all
      // blocks before or at "mid" are uninteresting.
      left = mid + 1;
    } else {
      // key at "target" is <= "mid". therefore all blocks
      // after "mid" are uninteresting.
      // if there is only one block left, we found it.
      if (left == right) break;
      right = mid;
    }
  }

  if (left == right) {
    // in one of the two following cases:
    // (1) left is the first one of block_ids
    // (2) there is a gap of blocks between block of `left` and `left-1`.
    // we can further distinguish the case of key in the block or key not
    // existing, by comparing the target key and the key of the previous
    // block to the left of the block found.
    if (block_ids[left] > 0 &&
        (left == left_bound || block_ids[left - 1] != block_ids[left] - 1) &&
        compareblockkey(block_ids[left] - 1, target) > 0) {
      current_ = restarts_;
      return false;
    }

    *index = block_ids[left];
    return true;
  } else {
    assert(left > right);
    // mark iterator invalid
    current_ = restarts_;
    return false;
  }
}

bool blockiter::hashseek(const slice& target, uint32_t* index) {
  assert(hash_index_);
  auto restart_index = hash_index_->getrestartindex(target);
  if (restart_index == nullptr) {
    current_ = restarts_;
    return false;
  }

  // the elements in restart_array[index : index + num_blocks]
  // are all with same prefix. we'll do binary search in that small range.
  auto left = restart_index->first_index;
  auto right = restart_index->first_index + restart_index->num_blocks - 1;
  return binaryseek(target, left, right, index);
}

bool blockiter::prefixseek(const slice& target, uint32_t* index) {
  assert(prefix_index_);
  uint32_t* block_ids = nullptr;
  uint32_t num_blocks = prefix_index_->getblocks(target, &block_ids);

  if (num_blocks == 0) {
    current_ = restarts_;
    return false;
  } else  {
    return binaryblockindexseek(target, block_ids, 0, num_blocks - 1, index);
  }
}

uint32_t block::numrestarts() const {
  assert(size_ >= 2*sizeof(uint32_t));
  return decodefixed32(data_ + size_ - sizeof(uint32_t));
}

block::block(const blockcontents& contents)
    : data_(contents.data.data()),
      size_(contents.data.size()),
      owned_(contents.heap_allocated),
      cachable_(contents.cachable),
      compression_type_(contents.compression_type) {
  if (size_ < sizeof(uint32_t)) {
    size_ = 0;  // error marker
  } else {
    restart_offset_ = size_ - (1 + numrestarts()) * sizeof(uint32_t);
    if (restart_offset_ > size_ - sizeof(uint32_t)) {
      // the size is too small for numrestarts() and therefore
      // restart_offset_ wrapped around.
      size_ = 0;
    }
  }
}

block::~block() {
  if (owned_) {
    delete[] data_;
  }
}

iterator* block::newiterator(
    const comparator* cmp, blockiter* iter, bool total_order_seek) {
  if (size_ < 2*sizeof(uint32_t)) {
    if (iter != nullptr) {
      iter->setstatus(status::corruption("bad block contents"));
      return iter;
    } else {
      return newerroriterator(status::corruption("bad block contents"));
    }
  }
  const uint32_t num_restarts = numrestarts();
  if (num_restarts == 0) {
    if (iter != nullptr) {
      iter->setstatus(status::ok());
      return iter;
    } else {
      return newemptyiterator();
    }
  } else {
    blockhashindex* hash_index_ptr =
        total_order_seek ? nullptr : hash_index_.get();
    blockprefixindex* prefix_index_ptr =
        total_order_seek ? nullptr : prefix_index_.get();

    if (iter != nullptr) {
      iter->initialize(cmp, data_, restart_offset_, num_restarts,
                    hash_index_ptr, prefix_index_ptr);
    } else {
      iter = new blockiter(cmp, data_, restart_offset_, num_restarts,
                           hash_index_ptr, prefix_index_ptr);
    }
  }

  return iter;
}

void block::setblockhashindex(blockhashindex* hash_index) {
  hash_index_.reset(hash_index);
}

void block::setblockprefixindex(blockprefixindex* prefix_index) {
  prefix_index_.reset(prefix_index);
}

size_t block::approximatememoryusage() const {
  size_t usage = size();
  if (hash_index_) {
    usage += hash_index_->approximatememoryusage();
  }
  if (prefix_index_) {
    usage += prefix_index_->approximatememoryusage();
  }
  return usage;
}

}  // namespace rocksdb

