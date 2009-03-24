# Finding valid warning flags for the C Compiler.           -*-Autoconf-*-
#
# Copyright (C) 2003 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA

# serial 2

# AC_COMPILE_STDERR_IFELSE([PROGRAM],
#                          [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ------------------------------------------------------------------
# Try to compile PROGRAM, and log the result of the compiler run.
m4_define([AC_COMPILE_STDERR_IFELSE],
[AC_LANG_COMPILER_REQUIRE()dnl
m4_ifvaln([$1], [AC_LANG_CONFTEST([$1])])dnl
rm -f conftest.$ac_objext
AS_IF([_AC_EVAL_STDERR($ac_compile) &&
	 AC_TRY_COMMAND([test -s conftest.$ac_objext])],
      [$2],
      [_AC_MSG_LOG_CONFTEST
m4_ifvaln([$3],[$3])dnl])dnl
rm -f conftest.$ac_objext m4_ifval([$1], [conftest.$ac_ext])[]dnl
])# AC_COMPILE_STDERR_IFELSE


# JRL_COMPILER_FLAGS_NAME
# ----------------------
# Return the name of the FLAGS variable for the current language.
m4_define([JRL_COMPILER_FLAGS_NAME],
[AC_LANG_CASE([C],   [CFLAGS],
              [C++], [CXXFLAGS])[]dnl
])# JRL_COMPILER_FLAGS_NAME


# JRL_COMPILER_OPTION_IF(OPTION, [IF-SUPPORTED], [IF-NOT-SUPPORTED])
# -----------------------------------------------------------------
# icc doesn't choke on unknown options, it will just issue warnings
# (even with -Werror).  So we grep stderr for any message that says an
# option was ignored.  For instance on "-Wall" it says:
#
#      icc: Command line warning: ignoring option '-W'; no argument required
#
# And GCC sometimes follows an irregular scheme (this is 3.3):
# g++: unrecognized option `-wd654'
# cc1plus: warning: "-Wbad-function-cast" is valid for C/ObjC but not for C++.
# cc1plus: warning: ignoring command line option '-Wbad-function-cast'
#
# Pay attention not to give grep a regexp starting with "-".
AC_DEFUN([JRL_COMPILER_OPTION_IF],
[AS_VAR_PUSHDEF([ac_Option],
                [ac_cv_[]_AC_LANG_ABBREV[]_supports_$1])dnl
AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1],
               [ac_Option],
[ac_save_[]JRL_COMPILER_FLAGS_NAME=$JRL_COMPILER_FLAGS_NAME
JRL_COMPILER_FLAGS_NAME="$JRL_COMPILER_FLAGS_NAME $1"
ac_Option=no
AC_COMPILE_STDERR_IFELSE([AC_LANG_PROGRAM],
  [if (grep -E 'm4_do([ignoring option],
                      [|ignoring command line option .$1.],
                      [|unrecognized option .*$1],
                      [|option.*$1.*not supported],
                      [|$1.* is valid for.*but not for])' conftest.err
      ) >/dev/null 2>&1; then :; else
      ac_Option=yes
   fi])
JRL_COMPILER_FLAGS_NAME=$ac_save_[]JRL_COMPILER_FLAGS_NAME
])
AS_IF([test AS_VAR_GET(ac_Option) = yes], [$2], [$3])[]dnl
AS_VAR_PUSHDEF([ac_Option])dnl
])# JRL_COMPILER_OPTION_IF


# JRL_CXX_WARNINGS(OPTIONS)
# ------------------------
# Check whether the C++ compiler support the OPTION, and if it
# does, append it to WARNING_CXXFLAGS.
# It is important to try the options all together, not just individually,
# as for instance for GCC 3.2 '-Wbad-function-cast' after '-W' gives:
#
# cc1plus: warning: ignoring command line option '-Wbad-function-cast'
#
# but not when used alone.  This also demonstrates the importance of the
# order.
AC_DEFUN([JRL_CXX_WARNINGS],
[ac_save_compiler_flags=$JRL_COMPILER_FLAGS_NAME
m4_foreach([AC_Option], [$1],
    [JRL_COMPILER_OPTION_IF(AC_Option,
           [WARNING_CXXFLAGS="$WARNING_CXXFLAGS AC_Option"
            JRL_COMPILER_FLAGS_NAME="$ac_save_compiler_flags $WARNING_FLAGS"])])
JRL_COMPILER_FLAGS_NAME=$ac_save_compiler_flags
AC_SUBST([WARNING_CXXFLAGS])
])# JRL_CXX_WARNINGS(OPTIONS)
