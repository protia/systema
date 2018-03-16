#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([systema], [1.0], [iocoder@aol.com])
AC_CONFIG_SRCDIR([])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lc':
AC_CHECK_LIB([c], [printf])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

# Initialize automake
AM_INIT_AUTOMAKE

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([compiler/Makefile])
AC_CONFIG_FILES([samples/Makefile])
AC_CONFIG_FILES([samples/hello/Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([misc/Makefile])
AC_OUTPUT

