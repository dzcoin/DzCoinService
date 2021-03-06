﻿tmp_dir="/tmp/rocksdb-sanity-test"

if [ "$#" -lt 2 ]; then
  echo "usage: ./auto_sanity_test.sh [new_commit] [old_commit]"
  echo "missing either [new_commit] or [old_commit], perform sanity check with the latest and 10th latest commits."
  recent_commits=`git log | grep -e "^commit [a-z0-9]\+$"| head -n10 | sed -e 's/commit //g'`
  commit_new=`echo "$recent_commits" | head -n1`
  commit_old=`echo "$recent_commits" | tail -n1`
  echo "the most recent commits are:"
  echo "$recent_commits"
else
  commit_new=$1
  commit_old=$2
fi

if [ ! -d $tmp_dir ]; then
  mkdir $tmp_dir
fi
dir_new="${tmp_dir}/${commit_new}"
dir_old="${tmp_dir}/${commit_old}"

function makestuff() {
  echo "make clean"
  make clean > /dev/null
  echo "make db_sanity_test -j32"
  make db_sanity_test -j32 > /dev/null
  if [ $? -ne 0 ]; then
    echo "[error] failed to perform 'make db_sanity_test'"
    exit 1
  fi
}

rm -r -f $dir_new
rm -r -f $dir_old

echo "running db sanity check with commits $commit_new and $commit_old."

echo "============================================================="
echo "making build $commit_new"
makestuff
mv db_sanity_test new_db_sanity_test
echo "creating db based on the new commit --- $commit_new"
./new_db_sanity_test $dir_new create

echo "============================================================="
echo "making build $commit_old"
makestuff
mv db_sanity_test old_db_sanity_test
echo "creating db based on the old commit --- $commit_old"
./old_db_sanity_test $dir_old create

echo "============================================================="
echo "verifying new db $dir_new using the old commit --- $commit_old"
./old_db_sanity_test $dir_new verify
if [ $? -ne 0 ]; then
  echo "[error] verification of $dir_new using commit $commit_old failed."
  exit 2
fi

echo "============================================================="
echo "verifying old db $dir_old using the new commit --- $commit_new"
./new_db_sanity_test $dir_old verify
if [ $? -ne 0 ]; then
  echo "[error] verification of $dir_old using commit $commit_new failed."
  exit 2
fi

rm old_db_sanity_test
rm new_db_sanity_test

echo "auto sanity test passed!"


