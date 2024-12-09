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

# Requires at least CMake 2.8.12 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_CXX_STANDARD 14)

SET(PROJECT_NAME roboptim-core)
SET(PROJECT_DESCRIPTION "RobOptim Core")
SET(PROJECT_URL "http://github.com/roboptim/roboptim-core")
SET(PROJECT_DEBUG_POSTFIX "_d")
set(PROJECT_USE_CMAKE_EXPORT TRUE)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/lapack.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/eigen.cmake)
INCLUDE(cmake/msvc-specific.cmake)

compute_project_args(PROJECT_ARGS LANGUAGES CXX C)
project(${PROJECT_NAME} ${PROJECT_ARGS})

# Use MathJax for Doxygen formulae
SET(DOXYGEN_USE_MATHJAX "YES")

SET(HEADERS
  ${CMAKE_SOURCE_DIR}/include/roboptim/core.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/alloc.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/cache.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/cache.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/multiplexer.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/multiplexer.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/wrapper.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/callback/wrapper.hxx
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
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result-analyzer.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result-analyzer.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result-with-warnings.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/result.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/scaling-helper.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/scaling-helper.hxx
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-callback.hh
  ${CMAKE_SOURCE_DIR}/include/roboptim/core/solver-callback.hxx
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

SET (ROBOPTIM_DO_NOT_CHECK_ALLOCATION TRUE CACHE BOOL
  "Do not check dynamic allocation of Eigen matrices")
IF(ROBOPTIM_DO_NOT_CHECK_ALLOCATION)
  ADD_DEFINITIONS(-DROBOPTIM_DO_NOT_CHECK_ALLOCATION)
  PKG_CONFIG_APPEND_CFLAGS (-DROBOPTIM_DO_NOT_CHECK_ALLOCATION)
ELSE()
  ADD_DEFINITIONS(-DEIGEN_RUNTIME_NO_MALLOC)
  PKG_CONFIG_APPEND_CFLAGS(-DEIGEN_RUNTIME_NO_MALLOC)
ENDIF()

# Fix for apparent bug with GCC 5.3
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  IF(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 5.3)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
  ENDIF()
ENDIF()

IF(PROJECT_DEBUG_POSTFIX)
  SET(FLAG_POSTFIX -DROBOPTIM_DEBUG_POSTFIX=${PROJECT_DEBUG_POSTFIX})
  ADD_COMPILE_OPTIONS($<$<CONFIG:Debug>:${FLAG_POSTFIX}>)
  PKG_CONFIG_APPEND_CFLAGS_DEBUG(${FLAG_POSTFIX})
ENDIF()

SET(USE_CXX11 TRUE CACHE BOOL "Whether to use C++11")

SET(ROBOPTIM_PRECOMPILE_DENSE_SPARSE FALSE CACHE BOOL
  "Precompile code for dense and sparse matrices (C++11 required)")
IF(ROBOPTIM_PRECOMPILE_DENSE_SPARSE)
  IF(USE_CXX11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ADD_DEFINITIONS(-DROBOPTIM_PRECOMPILED_DENSE_SPARSE)
    PKG_CONFIG_APPEND_CFLAGS(-DROBOPTIM_PRECOMPILED_DENSE_SPARSE)
  ELSE()
    MESSAGE(WARNING "C++11 is required for explicit template instantiation")
  ENDIF()
ENDIF()

IF(MSVC)
  ADD_DEFINITIONS(-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE)
  PKG_CONFIG_APPEND_CFLAGS (-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE)
ENDIF()

# If compiler support symbol visibility, enable it.
INCLUDE(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAS_VISIBILITY)
IF(HAS_VISIBILITY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
ENDIF()

# If we were given coverage options, we will only pass them to the core
# library.
SET(GENERATE_COVERAGE FALSE CACHE BOOL "Generate coverage data")
IF("${CMAKE_CXX_FLAGS}" MATCHES "--coverage")
  SET(GENERATE_COVERAGE TRUE CACHE BOOL "Generate coverage data" FORCE)
  STRING(REPLACE "--coverage" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  STRING(REPLACE "--coverage" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
  STRING(REPLACE "--coverage" "" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
ENDIF()

# Search for dependencies.
SET(BOOST_COMPONENTS date_time filesystem system unit_test_framework)
SEARCH_FOR_BOOST()
SEARCH_FOR_EIGEN("eigen3 >= 3.2.5")
add_project_dependency(Eigen3)

# Libtool dynamic loading
# This project does not use Libtool directly but still uses ltdl for
# plug-in loading.
IF(WIN32)
  ADD_OPTIONAL_DEPENDENCY("ltdl" "ltdl_d")
ENDIF(WIN32)

IF(NOT LTDL_FOUND)
  #FIXME: check that libltdl.so is available instead of adding it blindly.
  INCLUDE(CheckIncludeFileCXX)
  CHECK_INCLUDE_FILE_CXX(ltdl.h LTDL_H_FOUND)
  IF (NOT LTDL_H_FOUND)
    MESSAGE(FATAL_ERROR
      "Failed to find ltdl.h, check that Libtool ltdl is installed.")
  ENDIF()

  PKG_CONFIG_APPEND_LIBS(ltdl)
ENDIF(NOT LTDL_FOUND)

HEADER_INSTALL("${HEADERS}")

# Set plug-in subdirectory name (can be empty).
SET(PLUGIN_SUBDIR "${PROJECT_NAME}" CACHE
  STRING "libdir subdirectory where plug-ins will be installed (can be empty)")
SET(PLUGINDIR "${CMAKE_INSTALL_LIBDIR}/${PLUGIN_SUBDIR}")
SET(FULL_PLUGINDIR "${CMAKE_INSTALL_FULL_LIBDIR}/${PLUGIN_SUBDIR}")
MESSAGE("-- Relative plug-in directory: ${PLUGIN_SUBDIR}")
MESSAGE("-- Absolute plug-in directory: ${FULL_PLUGINDIR}")

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA "relplugindir=${PLUGIN_SUBDIR}\nplugindir=\${libdir}/\${relplugindir}")

# Set the default storage order for Eigen matrices (ColMajor or RowMajor).
# Note: RowMajor may yield better results for the dense Jacobian filling process
# if most functions only implement impl_gradient, but ColMajor is the usual
# default storage order for this kind of scenario.
SET(STORAGE_ORDER "ColMajor" CACHE
  STRING "Storage order for Eigen matrices (ColMajor or RowMajor")
IF(NOT (STORAGE_ORDER STREQUAL "ColMajor" OR STORAGE_ORDER STREQUAL "RowMajor"))
  MESSAGE(FATAL_ERROR "Storage order should be either ColMajor or RowMajor.")
ENDIF()

ADD_DEFINITIONS(-DROBOPTIM_STORAGE_ORDER=${STORAGE_ORDER})
PKG_CONFIG_APPEND_CFLAGS (-DROBOPTIM_STORAGE_ORDER=${STORAGE_ORDER})

OPTION(DISABLE_TESTS "Disable test programs" OFF)

# For MSVC, set local environment variable to enable finding the built dlls
# of roboptim-core and the plugins when launching ctest with RUN_TESTS
IF(MSVC)
  SET(CMAKE_MSVCIDE_RUN_PATH "\$(SolutionDir)/src/\$(Configuration)")
ENDIF(MSVC)

ADD_SUBDIRECTORY(src)

# Add main library to pkg-config file.
PKG_CONFIG_APPEND_LIBS(roboptim-core)

IF(NOT DISABLE_TESTS)
  ADD_SUBDIRECTORY(tests)
ELSE()
  MESSAGE(WARNING
    "Tests should only be disabled for specific cases. Do it at your own risk.")
ENDIF()


