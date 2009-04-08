# Don't expend macros beginning by JRL_.
m4_pattern_forbid([^JRL_])
AC_PREREQ([2.59])

## --------------------- ##
## Package configuration ##
## --------------------- ##
AC_INIT([Generic optimization library], [0.1], [thomas.moulard@gmail.com],
             [liboptimization])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])

# Automake.
AM_INIT_AUTOMAKE([1.9.6 dist-bzip2 -Wall nostdinc])

## ------------- ##
## C++ compiler  ##
## ------------- ##

# Search for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed-up compilation.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# Customize warnings.
JRL_CXX_WARNINGS([[-Wall],
                   [-Werror],
                   [-Wcast-align],
                   [-Wcast-qual],
                   [-Wformat],
                   [-Wmissing-prototypes],
                   [-Wstrict-prototypes],
                   [-Wwrite-strings],
                   [-Wbad-function-cast],
                   [-Wmissing-declarations],
                   [-Wnested-externs]])

## ------------------- ##
## Additionnal tools.  ##
## ------------------- ##

# Search for Libtool.
AC_CONFIG_MACRO_DIR([build-aux])
AC_PROG_LIBTOOL

# Search for Boost
BOOST_FUNCTION
BOOST_LAMBDA
BOOST_OPTIONAL
BOOST_UTILITY
BOOST_VARIANT

# Search for solvers (optional deps).
#  CFSQP
CFSQP_ARG_WITH
#  Ipopt
IPOPT_ARG_WITH


## ------------ ##
## Test-suite.  ##
## ------------ ##

AC_CONFIG_TESTDIR([tests])
AM_MISSING_PROG([AUTOM4TE], [autom4te])


## -------------- ##
## Write output.  ##
## -------------- ##

# Create config.h
AC_CONFIG_HEADERS([config.h])

# Create Makefiles.
AC_CONFIG_FILES([
  Makefile
   include/Makefile
   packages/Makefile
    packages/debian/Makefile
    packages/ebuild/Makefile
    packages/rpm/Makefile
   src/Makefile
   doc/Makefile
   tests/Makefile
    tests/atlocal
    tests/package.m4
])

AC_CONFIG_FILES([doc/Doxyfile])

# Write files.
AC_OUTPUT
