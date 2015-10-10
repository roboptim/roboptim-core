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

# Requires at least CMake 2.8 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/lapack.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/eigen.cmake)

SET(PROJECT_NAME roboptim-core)
SET(PROJECT_DESCRIPTION "RobOptim Core")
SET(PROJECT_URL "http://github.com/roboptim/roboptim-core")

# Use MathJax for Doxygen formulae
SET(DOXYGEN_USE_MATHJAX "YES")

SET(HEADERS
  ${CMAKE_SOURCE_DIR}/include/roboptim/core.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/alloc.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/cache.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/cache.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/multiplexer.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/multiplexer.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/debug.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivable-parametrized-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/derivative-size.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/detail/autopromote.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/detail/structured-input.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/detail/structured-input.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/detail/utility.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/differentiable-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/differentiable-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/decorator/cached-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/decorator/cached-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/decorator/finite-difference-gradient.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/decorator/finite-difference-gradient.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/bind.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/bind.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/chain.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/chain.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/concatenate.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/concatenate.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/derivative.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/map.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/map.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/minus.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/minus.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/plus.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/plus.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/product.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/product.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/scalar.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/scalar.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/selection-by-id.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/selection-by-id.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/selection.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/selection.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/split.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/operator/split.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/finite-difference-gradient.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function-pool.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function-pool.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/constant.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/cos.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/identity.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/polynomial.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/function/polynomial.hxx
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
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/optimization-logger.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/optimization-logger.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/parametrized-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/parametrized-function.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/plugin/dummy-laststate.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/plugin/dummy-laststate.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/plugin/dummy-td.hh
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
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-state.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-state.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-warning.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/sum-of-c1-squares.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/sum-of-c1-squares.hxx
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
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/matplotlib-commands.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/matplotlib-function.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/matplotlib-matrix.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/visualization/matplotlib.hh
  )

SETUP_PROJECT()

SET (ROBOPTIM_DO_NOT_CHECK_ALLOCATION TRUE CACHE BOOL
  "Do not check dynamic allocation of Eigen matrices")
IF(ROBOPTIM_DO_NOT_CHECK_ALLOCATION)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROBOPTIM_DO_NOT_CHECK_ALLOCATION")
  PKG_CONFIG_APPEND_CFLAGS (-DROBOPTIM_DO_NOT_CHECK_ALLOCATION)
ELSE()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEIGEN_RUNTIME_NO_MALLOC")
  PKG_CONFIG_APPEND_CFLAGS(-DEIGEN_RUNTIME_NO_MALLOC)
ENDIF()

IF(MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE")
  PKG_CONFIG_APPEND_CFLAGS (-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE)
ENDIF()

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
SET(BOOST_COMPONENTS
  date_time filesystem system thread program_options unit_test_framework)
SEARCH_FOR_BOOST()
ADD_REQUIRED_DEPENDENCY("eigen3 >= 3.2.0")
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

# Set the default storage order for Eigen matrices (ColMajor or RowMajor).
# Note: RowMajor may yield better results for the dense Jacobian filling process
# if most functions only implement impl_gradient, but ColMajor is the usual
# default storage order for this kind of scenario.
SET(STORAGE_ORDER "ColMajor" CACHE
  STRING "Storage order for Eigen matrices (ColMajor or RowMajor")
IF(NOT (STORAGE_ORDER STREQUAL "ColMajor" OR STORAGE_ORDER STREQUAL "RowMajor"))
  MESSAGE(FATAL_ERROR "Storage order should be either ColMajor or RowMajor.")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DROBOPTIM_STORAGE_ORDER=${STORAGE_ORDER}")
PKG_CONFIG_APPEND_CFLAGS (-DROBOPTIM_STORAGE_ORDER=${STORAGE_ORDER})

OPTION(DISABLE_TESTS "Disable test programs" OFF)

ADD_SUBDIRECTORY(src)

IF(NOT DISABLE_TESTS)
  ADD_SUBDIRECTORY(tests)
ELSE()
  MESSAGE(WARNING
    "Tests should only be disabled for speficic cases. Do it at your own risk.")
ENDIF()

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()
