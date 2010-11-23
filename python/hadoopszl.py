"""Command-line driver for executing Hadoop Sawzall jobs."""

import argparse
import logging
import os
import subprocess
import sys


__all__ = [
    "hadoop_pipes",
]


LOG = logging.getLogger(__name__)


def is_bad_szl(program):
    cmdline = "szl -execute=false %s" % (program,)
    LOG.info("Executing '%s'", cmdline)
    proc = subprocess.Popen(cmdline, shell=True)
    return proc.wait()


def hadoop_pipes(program, input_dir, output_dir, env):
    hadoop_opts = env["hadoop_opts"]
    hadoop_opts["hadoop.pipes.java.recordreader"] = "true"
    hadoop_opts["hadoop.pipes.java.recordwriter"] = "true"
    hadoop_opts["hadoop.pipes.command-file.keep"] = "true"

    cmdline = ["hadoop", "pipes"]
    for opt in sorted(hadoop_opts):
        cmdline.append("-D%s=%s" % (opt, hadoop_opts[opt]))
    cmdline.append("-files")
    cmdline.append(program)
    cmdline.append("-program")
    cmdline.append("file://%s" % (env["hadoop_szl_runner"],))
    cmdline.append("-input")
    cmdline.append(input_dir)
    cmdline.append("-output")
    cmdline.append(output_dir)
    cmdline = " ".join(cmdline)
    LOG.info("Executing '%s'", cmdline)

    proc = subprocess.Popen(cmdline, shell=True)
    return proc.wait()


def main(hadoop_szl_runner=None):
    p = argparse.ArgumentParser(prog="hadoop-saw", description=__doc__.strip())
    p.add_argument("program",
                   metavar="<program>",
                   help="path to the Sawzal program")
    p.add_argument("input_dir",
                   metavar="<input-dir>",
                   help="path to input directory")
    p.add_argument("output_dir",
                   metavar="<output-dir>",
                   help="path to output directory")

    args = p.parse_args()

    logging.basicConfig(level=logging.DEBUG,
                        format="%(message)s")

    if hadoop_szl_runner is None:
        try:
            hadoop_szl_runner = os.environ["HADOOP_SZL_RUNNER"]
        except KeyError:
            LOG.error("No Hadoop Sawzall runner specified")
            return 1

    env = {
        "hadoop_szl_runner": hadoop_szl_runner,
        "hadoop_opts": {}
    }

    if is_bad_szl(args.program):
        return 1

    return hadoop_pipes(args.program, args.input_dir, args.output_dir, env)


if __name__ == "__main__":
    sys.exit(main())
