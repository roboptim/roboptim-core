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

# Declare the CHECK_PREFIX option.
# It allows the user to instrument the test suite easily to make it run
# through Valgrind for instance.
SET(CHECK_PREFIX "" CACHE STRING "Shell prefix used when running tests")
SEPARATE_ARGUMENTS(CHECK_PREFIX)

# Make sure local headers are found
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/tests)


# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Make Boost.Test generates the main function in test cases.
IF(WIN32)
  ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
ENDIF(WIN32)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

# To locate built plug-in.
# First, determine the build subdirectory according to the build type
# [TODO] This could be a macro in jrl-cmake-modules
IF(WIN32)
  SET(BUILD_SUBDIRECTORY 
      "/src/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>$<$<CONFIG:MinSizeRel>:MinSizeRel>"
	 )
ELSE()
  SET(BUILD_SUBDIRECTORY "/src")
ENDIF(WIN32)
# Second, add the path
ADD_COMPILE_OPTIONS(-DPLUGIN_PATH="${CMAKE_BINARY_DIR}${BUILD_SUBDIRECTORY}")

# To locate reference logs
ADD_DEFINITIONS(-DTESTS_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# ROBOPTIM_CORE_TEST(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# Boost and add it to the test suite.
# Complementary source files can be added as extra arguments.
#
SET(CMAKE_VERBOSE_MAKEFILE ON)
MACRO(ROBOPTIM_CORE_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cc ${ARGN})
  ADD_TEST(NAME ${NAME}
    COMMAND ${CHECK_PREFIX} ${CMAKE_CURRENT_BINARY_DIR}/${NAME})

  #Adding a project configuration file (for MSVC only)
  GENERATE_MSVC_DOT_USER_FILE(${NAME})

  target_include_directories(
    ${NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/tests)
  TARGET_LINK_LIBRARIES(${NAME} roboptim-core)
  IF(NOT LTDL_FOUND)
    TARGET_LINK_LIBRARIES(${NAME} ltdl)
  ELSE()
    PKG_CONFIG_USE_DEPENDENCY(${NAME} ltdl)
  ENDIF(NOT LTDL_FOUND)

  # Link against Boost.
  TARGET_LINK_LIBRARIES(${NAME} ${Boost_LIBRARIES})

  # Make sure the plugins will be found.
  # Note: this does not append the environment variables!
  IF(NOT WIN32)
  SET_PROPERTY(
    TEST ${NAME} PROPERTY ENVIRONMENT
    "LTDL_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src:$ENV{LTDL_LIBRARY_PATH} "
    "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src/:$ENV{LD_LIBRARY_PATH}")
  ENDIF(NOT WIN32)
ENDMACRO(ROBOPTIM_CORE_TEST)

# Basic types.
ROBOPTIM_CORE_TEST(interval)
ROBOPTIM_CORE_TEST(util)
ROBOPTIM_CORE_TEST(terminal-color)
ROBOPTIM_CORE_TEST(indent)
ROBOPTIM_CORE_TEST(detail-utility)
ROBOPTIM_CORE_TEST(detail-autopromote)
ROBOPTIM_CORE_TEST(detail-structured-input)
ROBOPTIM_CORE_TEST(cache)
ROBOPTIM_CORE_TEST(function)
ROBOPTIM_CORE_TEST(derivable-function)
ROBOPTIM_CORE_TEST(twice-derivable-function)
ROBOPTIM_CORE_TEST(quadratic-function)
ROBOPTIM_CORE_TEST(linear-function)
ROBOPTIM_CORE_TEST(function-typing)
ROBOPTIM_CORE_TEST(function-pool)
ROBOPTIM_CORE_TEST(problem)
ROBOPTIM_CORE_TEST(problem-cc)
ROBOPTIM_CORE_TEST(numeric-linear-function)
ROBOPTIM_CORE_TEST(numeric-quadratic-function)
ROBOPTIM_CORE_TEST(n-times-derivable-function)
ROBOPTIM_CORE_TEST(parametrized-function)
ROBOPTIM_CORE_TEST(derivable-parametrized-function)
ROBOPTIM_CORE_TEST(storage-order)
ROBOPTIM_CORE_TEST(ref)

# Solver.
ROBOPTIM_CORE_TEST(solver)
ROBOPTIM_CORE_TEST(solver-factory)

# Compile the dummy solver in this test to bypass the plug-in system.
ROBOPTIM_CORE_TEST(simple "${CMAKE_SOURCE_DIR}/src/dummy.cc")

# Results.
ROBOPTIM_CORE_TEST(result)
ROBOPTIM_CORE_TEST(result-with-warnings)
ROBOPTIM_CORE_TEST(solver-error)
ROBOPTIM_CORE_TEST(result-analyzer)

# Helpers.
ROBOPTIM_CORE_TEST(scaling-helper)

# Example from documentation.
ROBOPTIM_CORE_TEST(example)

# Dynamic loading mechanism.
ROBOPTIM_CORE_TEST(plugin)

# Dynamic loading mechanism with solver's last state
ROBOPTIM_CORE_TEST(plugin-laststate)

# Callbacks.
ROBOPTIM_CORE_TEST(solver-state)
ROBOPTIM_CORE_TEST(optimization-logger)
ROBOPTIM_CORE_TEST(multiplexer)

# Built-in mathematical functions.
ROBOPTIM_CORE_TEST(function-constant)
ROBOPTIM_CORE_TEST(function-cos)
ROBOPTIM_CORE_TEST(function-identity)
ROBOPTIM_CORE_TEST(function-sin)
ROBOPTIM_CORE_TEST(function-polynomial)

# Sum Of C1 Squares.
ROBOPTIM_CORE_TEST(sum-of-c1-squares)

# Decorators.
ROBOPTIM_CORE_TEST(decorator-cached-function)
ROBOPTIM_CORE_TEST(decorator-finite-difference-gradient)
ROBOPTIM_CORE_TEST(decorator-finite-difference-jacobian)

# Operators.
ROBOPTIM_CORE_TEST(operator-bind)
ROBOPTIM_CORE_TEST(operator-chain)
ROBOPTIM_CORE_TEST(operator-concatenate)
ROBOPTIM_CORE_TEST(operator-derivative)
ROBOPTIM_CORE_TEST(operator-map)
ROBOPTIM_CORE_TEST(operator-minus)
ROBOPTIM_CORE_TEST(operator-plus)
ROBOPTIM_CORE_TEST(operator-product)
ROBOPTIM_CORE_TEST(operator-scalar)
ROBOPTIM_CORE_TEST(operator-selection)
ROBOPTIM_CORE_TEST(operator-selection-by-id)
ROBOPTIM_CORE_TEST(operator-split)

# Visualization
ROBOPTIM_CORE_TEST(visualization-gnuplot-simple)
ROBOPTIM_CORE_TEST(visualization-gnuplot-function)
ROBOPTIM_CORE_TEST(visualization-gnuplot-differentiable-function)
ROBOPTIM_CORE_TEST(visualization-matplotlib-simple)
ROBOPTIM_CORE_TEST(visualization-matplotlib-function)
ROBOPTIM_CORE_TEST(visualization-matplotlib-multiplot)
ROBOPTIM_CORE_TEST(visualization-matplotlib-matrix)
