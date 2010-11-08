ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = hadoop-szl-runner

hadoop_szl_runner_SOURCES = \
	hadoop-szl/main.cc \
	hadoop-szl/map.h \
	hadoop-szl/map.cc \
	hadoop-szl/reduce.h \
	hadoop-szl/reduce.cc

hadoop_szl_runner_CPPFLAGS = $(SZL_CFLAGS) $(HADOOP_PIPES_CFLAGS)
hadoop_szl_runner_LDADD = $(SZL_LIBS) $(HADOOP_PIPES_LIBS)
