ACLOCAL_AMFLAGS = -I m4

libexec_PROGRAMS = hadoop-szl-runner

hadoop_szl_runner_SOURCES = \
	hadoop-szl/hadoop-szl-runner.cc \
	hadoop-szl/map.h \
	hadoop-szl/map.cc \
	hadoop-szl/reduce.h \
	hadoop-szl/reduce.cc

hadoop_szl_runner_CPPFLAGS = $(SZL_CFLAGS) $(HADOOP_PIPES_CFLAGS)
hadoop_szl_runner_LDADD = $(SZL_LIBS) $(HADOOP_PIPES_LIBS)

bin_SCRIPTS = scripts/hadoop-saw

scripts/hadoop-saw: scripts/hadoop-saw.in

python_PYTHON = \
	python/hadoopszl.py

TESTS = \
	test/wc/run.sh

TESTS_ENVIRONMENT = \
	pythonpath=$(abs_top_srcdir)/python \
	hadoop_szl_runner=$(abs_top_builddir)/hadoop-szl-runner \
	PATH=$(abs_top_builddir)/scripts:$$PATH

all-local:
	chmod 0755 scripts/hadoop-saw

EXTRA_DIST = \
	$(TESTS) \
	test/wc/lorem.txt \
	test/wc/wc.conf \
	test/wc/wc.szl \
	scripts/hadoop-saw.in
