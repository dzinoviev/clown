# Process this file with autoconf to produce a configure script.

AC_INIT([clown], [2.10], [dmitry@mcs.suffolk.edu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_CXX

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_CHECK_TYPES([struct termios, struct termio], [], [], [[#include <term.h>]])

AC_SUBST([AM_CFLAGS], ["-Wno-unused-result -Wall -Werror -fpic -Wall -Wextra"])
AC_SUBST([AM_CPPFLAGS], [-I../include/])
AC_SUBST([AM_CXXFLAGS], ["-Wno-unused-result -Wall -Werror -fpic -Wall -Wextra"])

# Supplied by autoscan
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h libintl.h limits.h linux/limits.h malloc.h stddef.h stdlib.h string.h sys/ioctl.h sys/time.h unistd.h])
AC_CHECK_LIB([pthread], [pthread_mutex_lock])
AC_CHECK_LIB([readline], [main])
AC_CHECK_FUNCS([atexit getcwd memset select strdup strrchr strtol readline add_history])

AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
        include/Makefile
        codelib/Makefile
        devlib/Makefile
        clown/Makefile
        cas/Makefile
        clink/Makefile
        cdiskdup/Makefile
        cformat/Makefile
        man/Makefile
        examples/Makefile
])
AC_OUTPUT
