#!/bin/sh

srcdir="$(cd $(dirname $0) && pwd)"

output=$(hadoop-saw \
  $srcdir/bad-syntax.szl \
  hdfs:/fake-input-dir \
  hdfs:/fake-output-dir 2>&1)

error_msg="foo undeclared"
echo "$output" | grep "$error_msg" > /dev/null
if [ "$?" != "0" ] ; then
  echo "String \"$error_msg\" not found in szl output:"
  echo $output
  exit 1
fi
