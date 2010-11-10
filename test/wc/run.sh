#!/bin/sh

srcdir="$(cd $(dirname $0) && pwd)"
job_dir="hdfs:/user/$USER/hadoop-szl/test/$(basename $srcdir)"
input_dir="$job_dir/in"
output_dir="$job_dir/out"

hadoop fs -rmr $job_dir
hadoop fs -mkdir $input_dir
hadoop fs -put $srcdir/*.txt $input_dir

hadoop-saw \
  /$srcdir/wc.szl \
  $input_dir \
  $output_dir
