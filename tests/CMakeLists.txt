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

# Generate local-libdir.hh.
CONFIG_FILES(local-libdir.hh)

# Make sure local headers are found
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/tests)


# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Make Boost.Test generates the main function in test cases.
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)


# ROBOPTIM_CORE_TEST(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# Boost and add it to the test suite.
#
MACRO(ROBOPTIM_CORE_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cc)
  ADD_TEST(${NAME} ${RUNTIME_OUTPUT_DIRECTORY}/${NAME})

  TARGET_LINK_LIBRARIES(${NAME} roboptim-core)

  # Link against Boost.
  TARGET_LINK_LIBRARIES(${NAME} ${Boost_LIBRARIES})
ENDMACRO(ROBOPTIM_CORE_TEST)


# simple
# Link the dummy solver with this test to bypass the plug-in system.
ADD_EXECUTABLE(simple
  simple.cc
  ${CMAKE_SOURCE_DIR}/src/roboptim-core-dummy-plugin.cc)
TARGET_LINK_LIBRARIES(simple roboptim-core ${Boost_LIBRARIES})

# Basic types.
ROBOPTIM_CORE_TEST(interval)
ROBOPTIM_CORE_TEST(util)
ROBOPTIM_CORE_TEST(result)
ROBOPTIM_CORE_TEST(function)
ROBOPTIM_CORE_TEST(derivable-function)
ROBOPTIM_CORE_TEST(twice-derivable-function)
ROBOPTIM_CORE_TEST(quadratic-function)
ROBOPTIM_CORE_TEST(linear-function)
ROBOPTIM_CORE_TEST(problem-cc)
ROBOPTIM_CORE_TEST(numeric-linear-function)
ROBOPTIM_CORE_TEST(numeric-quadratic-function)
ROBOPTIM_CORE_TEST(n-times-derivable-function)
ROBOPTIM_CORE_TEST(parametrized-function)
ROBOPTIM_CORE_TEST(derivable-parametrized-function)

# Dynamic loading mechanism.
ROBOPTIM_CORE_TEST(plugin)

# Algorithm.
ROBOPTIM_CORE_TEST(finite-difference-gradient)

# Built-in mathematical functions.
ROBOPTIM_CORE_TEST(identity-function)
ROBOPTIM_CORE_TEST(constant-function)

ROBOPTIM_CORE_TEST(cached-function)
ROBOPTIM_CORE_TEST(split)

# Visualization
ROBOPTIM_CORE_TEST(visualization-gnuplot-simple)

# visualization-gnuplot-function
ROBOPTIM_CORE_TEST(visualization-gnuplot-function)
