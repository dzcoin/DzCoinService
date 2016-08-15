﻿//  copyright (c) 2013, facebook, inc.  all rights reserved.
//  this source code is licensed under the bsd-style license found in the
//  license file in the root directory of this source tree. an additional grant
//  of patent rights can be found in the patents file in the same directory.

#pragma once
#include <string>
#include <algorithm>

#include "rocksdb/utilities/spatial_db.h"

namespace rocksdb {
namespace spatial {

// indexing idea from http://msdn.microsoft.com/en-us/library/bb259689.aspx
inline uint64_t gettilefromcoord(double x, double start, double end,
                                 uint32_t tile_bits) {
  if (x < start) {
    return 0;
  }
  uint64_t tiles = static_cast<uint64_t>(1) << tile_bits;
  uint64_t r = ((x - start) / (end - start)) * tiles;
  return std::min(r, tiles - 1);
}

inline uint64_t getquadkeyfromtile(uint64_t tile_x, uint64_t tile_y,
                                   uint32_t tile_bits) {
  uint64_t quad_key = 0;
  for (uint32_t i = 0; i < tile_bits; ++i) {
    uint32_t mask = (1ll << i);
    quad_key |= (tile_x & mask) << i;
    quad_key |= (tile_y & mask) << (i + 1);
  }
  return quad_key;
}

inline boundingbox<uint64_t> gettileboundingbox(
    const spatialindexoptions& spatial_index, boundingbox<double> bbox) {
  return boundingbox<uint64_t>(
      gettilefromcoord(bbox.min_x, spatial_index.bbox.min_x,
                       spatial_index.bbox.max_x, spatial_index.tile_bits),
      gettilefromcoord(bbox.min_y, spatial_index.bbox.min_y,
                       spatial_index.bbox.max_y, spatial_index.tile_bits),
      gettilefromcoord(bbox.max_x, spatial_index.bbox.min_x,
                       spatial_index.bbox.max_x, spatial_index.tile_bits),
      gettilefromcoord(bbox.max_y, spatial_index.bbox.min_y,
                       spatial_index.bbox.max_y, spatial_index.tile_bits));
}

// big endian can be compared using memcpy
inline void putfixed64bigendian(std::string* dst, uint64_t value) {
  char buf[sizeof(value)];
  buf[0] = (value >> 56) & 0xff;
  buf[1] = (value >> 48) & 0xff;
  buf[2] = (value >> 40) & 0xff;
  buf[3] = (value >> 32) & 0xff;
  buf[4] = (value >> 24) & 0xff;
  buf[5] = (value >> 16) & 0xff;
  buf[6] = (value >> 8) & 0xff;
  buf[7] = value & 0xff;
  dst->append(buf, sizeof(buf));
}

// big endian can be compared using memcpy
inline bool getfixed64bigendian(const slice& input, uint64_t* value) {
  if (input.size() < sizeof(uint64_t)) {
    return false;
  }
  auto ptr = input.data();
  *value = (static_cast<uint64_t>(static_cast<unsigned char>(ptr[0])) << 56) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[1])) << 48) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[2])) << 40) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[3])) << 32) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[4])) << 24) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[5])) << 16) |
           (static_cast<uint64_t>(static_cast<unsigned char>(ptr[6])) << 8) |
           static_cast<uint64_t>(static_cast<unsigned char>(ptr[7]));
  return true;
}

inline void putdouble(std::string* dst, double d) {
  dst->append(reinterpret_cast<char*>(&d), sizeof(double));
}

inline bool getdouble(slice* input, double* d) {
  if (input->size() < sizeof(double)) {
    return false;
  }
  memcpy(d, input->data(), sizeof(double));
  input->remove_prefix(sizeof(double));
  return true;
}

}  // namespace spatial
}  // namespace rocksdb

