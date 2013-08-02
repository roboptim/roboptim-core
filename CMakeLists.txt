# Copyright 2010, Thomas Moulard, LAAS-CNRS
#
# This file is part of roboptim-core.
# roboptim-core is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# roboptim-core is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Lesser Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with roboptim-core.  If not, see <http://www.gnu.org/licenses/>.

# Requires at least CMake 2.6 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/lapack.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/eigen.cmake)

SET(PROJECT_NAME roboptim-core)
SET(PROJECT_DESCRIPTION "RobOptim Core")
SET(PROJECT_URL "http://github.com/roboptim/roboptim-core")

SET(HEADERS
  ${CMAKE_SOURCE_DIR}/include/roboptim/core.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/debug.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivable-parametrized-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivative-size.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/detail/autopromote.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/differentiable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/differentiable-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/cached-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/cached-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/chain.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/chain.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/derivative.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/minus.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/minus.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/plus.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/plus.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/selection.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/selection.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/split.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/filter/split.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/finite-difference-gradient.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/finite-difference-gradient.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/constant.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/cos.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/identity.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/sin.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/fwd.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/generic-solver.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/indent.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/io.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/linear-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/linear-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/n-times-derivable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/n-times-derivable-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/numeric-linear-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/numeric-linear-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/numeric-quadratic-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/numeric-quadratic-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/parametrized-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/parametrized-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/plugin/dummy-laststate.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/plugin/dummy.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/portability.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/problem.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/problem.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/quadratic-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/quadratic-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result-with-warnings.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-error.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-factory.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-factory.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-warning.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/sum-of-c1-squares.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/sys.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/terminal-color.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/twice-derivable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/twice-differentiable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/twice-differentiable-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/util.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/util.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/fwd.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/gnuplot-commands.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/gnuplot-differentiable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/gnuplot-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/gnuplot-matrix.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/gnuplot.hh
  )

SETUP_PROJECT()

# If compiler support symbol visibility, enable it.
INCLUDE(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAS_VISIBILITY)
IF(HAS_VISIBILITY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
ENDIF()

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA "plugindir=${CMAKE_INSTALL_FULL_LIBDIR}/roboptim-core")

# Add main library to pkg-config file.
PKG_CONFIG_APPEND_LIBS(roboptim-core)

# Search for dependencies.
SEARCH_FOR_BOOST()
ADD_REQUIRED_DEPENDENCY("eigen3 >= 3.1.0")
ADD_REQUIRED_DEPENDENCY("liblog4cxx >= 0.10.0")

# Libtool dynamic loading
# This project does not use Libtool directly but still uses ltdl for
# plug-in loading.
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(ltdl.h LTDL_H_FOUND)
IF (NOT LTDL_H_FOUND)
  MESSAGE(FATAL_ERROR
    "Failed to find ltdl.h, check that Libtool ltdl is installed.")
ENDIF()
#FIXME: check that libltdl.so is available instead of adding it blindly.
PKG_CONFIG_APPEND_LIBS(ltdl)

HEADER_INSTALL("${HEADERS}")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()
