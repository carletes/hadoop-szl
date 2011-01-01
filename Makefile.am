ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = libhadoop-szl-impl.la

libhadoop_szl_impl_la_SOURCES =\
	hadoop-szl/base64.h \
	hadoop-szl/base64.cc \
	hadoop-szl/emitter.h \
	hadoop-szl/emitter.cc \
	hadoop-szl/runner.h \
	hadoop-szl/runner.cc \
	hadoop-szl/table.h \
	hadoop-szl/table.cc

libhadoop_szl_impl_la_CPPFLAGS = $(SZL_CFLAGS)

libexec_PROGRAMS = hadoop-szl-runner

hadoop_szl_runner_SOURCES = \
	hadoop-szl/hadoop-szl-runner.cc \
	hadoop-szl/map.h \
	hadoop-szl/map.cc \
	hadoop-szl/reduce.h \
	hadoop-szl/reduce.cc

hadoop_szl_runner_CPPFLAGS = $(SZL_CFLAGS) $(HADOOP_PIPES_CFLAGS)
hadoop_szl_runner_LDADD = \
	libhadoop-szl-impl.la \
	$(SZL_LIBS) \
	$(SZL_EMITTERS_LIBS) \
	$(SZL_INTRINSICS_LIBS) \
	$(HADOOP_PIPES_LIBS)

python_PYTHON = \
	python/hadoopszl.py

bin_SCRIPTS = scripts/hadoop-saw

scripts/hadoop-saw: scripts/hadoop-saw.in

test/run-test.sh: test/run-test.sh.in

TESTS = \
	test/word-freq/word-freq.szl \
	test/wc/wc.szl

TESTS_ENVIRONMENT = test/run-test.sh

all-local:
	chmod 0755 test/run-test.sh
	chmod 0755 scripts/hadoop-saw

EXTRA_DIST = \
	test/run-test.sh \
	$(TESTS) \
	test/bad-syntax/bad-syntax.szl \
	test/wc/lorem.txt \
	test/wc/wc.conf \
	test/wc/wc.szl \
	scripts/hadoop-saw.in
