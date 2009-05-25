# Don't expend macros beginning by JRL_.
m4_pattern_forbid([^JRL_])
AC_PREREQ([2.59])

## --------------------- ##
## Package configuration ##
## --------------------- ##
AC_INIT([RobOptim core library], [0.2], [roboptim-user@lists.sourceforge.net],
        [roboptim-core])

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

# Macros are stored in build-aux.
AC_CONFIG_MACRO_DIR([build-aux])

## ------------------- ##
## Additionnal tools.  ##
## ------------------- ##

# Search for misc. common tools.
AC_PROG_MKDIR_P
AC_PROG_SED

AC_ARG_VAR([DOXYGEN], [the Doxygen documentation generation program])
AC_CHECK_PROGS([DOXYGEN], [doxygen])

# Search for Libtool.
AC_PROG_LIBTOOL

# Search for libltdl.
AC_CHECK_LIB([ltdl], [lt_dladvise_init], [],
		     [AC_MSG_ERROR([installed libltdl is too old])])


# Search for Boost
BOOST_FORMAT
BOOST_MPL
BOOST_OPTIONAL
BOOST_STATICASSERT
BOOST_TUPLE
BOOST_TYPETRAITS
BOOST_UTILITY
BOOST_VARIANT

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
   src/Makefile
   doc/Makefile
   tests/Makefile
    tests/atlocal
    tests/package.m4
])

AC_CONFIG_FILES([doc/Doxyfile])

AC_CONFIG_FILES([doc/sf-upload.sh], [chmod +x doc/sf-upload.sh])

# Write files.
AC_OUTPUT
