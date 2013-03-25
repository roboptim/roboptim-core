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

# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Define the directory where plug-ins will be installed.
SET(PLUGINDIR lib/${PROJECT_NAME})

# Main library.
ADD_LIBRARY(roboptim-core SHARED
  ${HEADERS}
  constant-function.cc
  debug.hh
  debug.cc
  doc.hh
  finite-difference-gradient.cc
  function.cc
  generic-solver.cc
  identity-function.cc
  indent.cc
  numeric-linear-function.cc
  numeric-quadratic-function.cc
  result.cc
  result-with-warnings.cc
  solver.cc
  solver-error.cc
  solver-warning.cc
  sum-of-c1-squares.cc
  util.cc

  visualization/gnuplot.cc
  visualization/gnuplot-commands.cc
  )

PKG_CONFIG_USE_DEPENDENCY(roboptim-core eigen3)
PKG_CONFIG_USE_DEPENDENCY(roboptim-core liblog4cxx)

TARGET_LINK_LIBRARIES(roboptim-core ltdl)
SET_TARGET_PROPERTIES(roboptim-core PROPERTIES SOVERSION 1.1.0)
INSTALL(TARGETS roboptim-core DESTINATION lib)


# Dummy plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy MODULE dummy.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy PROPERTIES
  PREFIX ""
  SOVERSION 1.1.0)
INSTALL(TARGETS roboptim-core-plugin-dummy DESTINATION ${PLUGINDIR})

# Dummy-laststate plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy-laststate MODULE dummy-laststate.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy-laststate roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy-laststate roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-laststate PROPERTIES
  PREFIX ""
  SOVERSION 1.1.0)
INSTALL(TARGETS roboptim-core-plugin-dummy-laststate DESTINATION ${PLUGINDIR})
