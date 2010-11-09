ACLOCAL_AMFLAGS = -I m4

libexec_PROGRAMS = hadoop-szl-runner

hadoop_szl_runner_SOURCES = \
	hadoop-szl/main.cc \
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

EXTRA_DIST = \
	scripts/hadoop-saw.in
