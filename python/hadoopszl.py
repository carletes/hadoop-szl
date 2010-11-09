"""Command-line driver for executing Hadoop Sawzall jobs."""

import argparse
import logging
import os
import sys


__all__ = [
    "hadoop_pipes",
]


LOG = logging.getLogger(__name__)


def hadoop_pipes(program, input_dir, output_dir, **env):
    """
    #!/bin/sh

    set -eux

    top_srcdir=$(cd $(dirname $0) && pwd)
    top_builddir=$(pwd)

    job_dir="/user/carlos/hadoop-szl"
    input_dir="$job_dir/in"
    output_dir="$job_dir/out"

    make all install

    hadoop fs -rmr $job_dir
    for d in $input_dir; do
      hadoop fs -mkdir $d
    done

    hadoop fs -put \
      $top_srcdir/access_log-head \
      $input_dir

    program_file=$top_builddir/hadoop-szl-runner

    hadoop \
      pipes \
          -Dhadoop.pipes.java.recordreader=true \
          -Dhadoop.pipes.java.recordwriter=true \
          -program file://$program_file \
          -input hdfs://$input_dir \
          -output hdfs://$output_dir
    """


def main(hadoop_szl_runner=None):
    p = argparse.ArgumentParser(prog="hadoop-saw", description=__doc__.strip())
    p.add_argument("program",
                   metavar="<program>",
                   help="path to the Sawzal program")
    p.add_argument("input-dir",
                   metavar="<input-dir>",
                   help="path to input directory")
    p.add_argument("output-dir",
                   metavar="<output-dir>",
                   help="path to output directory")
    p.add_argument("--job-server",
                   metavar="<host:port>",
                   help="address of the job server to use")
    p.add_argument("--num-reducers",
                   type=int,
                   metavar="<n>",
                   help="number of reduce tasks (default: %(default)s)",
                   default=2)

    args = p.parse_args()

    logging.basicConfig(level=logging.WARN,
                        format="%(message)s")

    if hadoop_szl_runner is None:
        try:
            hadoop_szl_runner = os.environ["HADOOP_SZL_RUNNER"]
        except KeyError:
            LOG.error("No Hadoop Sawzall runner specified")
            return 1


if __name__ == "__main__":
    sys.exit(main())
