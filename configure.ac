AC_PREREQ([2.61])

m4_define([hadoop_szl_major], [1])
m4_define([hadoop_szl_minor], [0])
m4_define([hadoop_szl_version],
          m4_format([%d.%d], hadoop_szl_major, hadoop_szl_minor))

AC_INIT([Hadoop interface to Sawzall], hadoop_szl_version,
        [carlos@pepelabs.net], [hadoop-szl])
AC_CONFIG_SRCDIR([configure])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.9 foreign tar-ustar])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG([C++])

# Sawzall
PKG_CHECK_MODULES([SZL], [szl >= 1.0])
AC_SUBST(SZL_CFLAGS)
AC_SUBST(SZL_LIBS)

# Hadoop Pipes
HADOOP_PIPES_CFLAGS=""
HADOOP_PIPES_LIBS="-lhadooppipes -lhadooputils"
AC_SUBST(HADOOP_PIPES_CFLAGS)
AC_SUBST(HADOOP_PIPES_LIBS)

AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
