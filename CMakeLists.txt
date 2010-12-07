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

SET(PROJECT_NAME roboptim-core)
SET(PROJECT_DESCRIPTION "RobOptim core")
SET(PROJECT_URL "http://github.com/laas/roboptim-core")

SET(${PROJECT_NAME}_HEADERS
  include/roboptim/core/result.hh
  include/roboptim/core/identity-function.hh
  include/roboptim/core/solver-factory.hxx
  include/roboptim/core/problem.hxx
  include/roboptim/core/util.hxx
  include/roboptim/core/n-times-derivable-function.hh
  include/roboptim/core/solver.hxx
  include/roboptim/core/sys.hh
  include/roboptim/core/function.hh
  include/roboptim/core/portability.hh
  include/roboptim/core/parametrized-function.hxx
  include/roboptim/core/numeric-quadratic-function.hh
  include/roboptim/core/io.hh
  include/roboptim/core/solver-error.hh
  include/roboptim/core/linear-function.hh
  include/roboptim/core/twice-derivable-function.hh
  include/roboptim/core/indent.hh
  include/roboptim/core/constant-function.hh
  include/roboptim/core/generic-solver.hh
  include/roboptim/core/fwd.hh
  include/roboptim/core/problem.hh
  include/roboptim/core/derivative-size.hh
  include/roboptim/core/plugin/dummy.hh
  include/roboptim/core/debug.hh
  include/roboptim/core/solver-factory.hh
  include/roboptim/core/derivable-function.hh
  include/roboptim/core/finite-difference-gradient.hxx
  include/roboptim/core/finite-difference-gradient.hh
  include/roboptim/core/solver-warning.hh
  include/roboptim/core/numeric-linear-function.hh
  include/roboptim/core/n-times-derivable-function.hxx
  include/roboptim/core/visualization/gnuplot-function.hh
  include/roboptim/core/visualization/fwd.hh
  include/roboptim/core/visualization/gnuplot.hh
  include/roboptim/core/visualization/gnuplot-commands.hh
  include/roboptim/core/solver.hh
  include/roboptim/core/result-with-warnings.hh
  include/roboptim/core/parametrized-function.hh
  include/roboptim/core/quadratic-function.hh
  include/roboptim/core/derivable-parametrized-function.hh
  include/roboptim/core/filter/split.hh
  include/roboptim/core/filter/cached-function.hxx
  include/roboptim/core/filter/split.hxx
  include/roboptim/core/filter/cached-function.hh
  include/roboptim/core/util.hh
  include/roboptim/core.hh
  )

SETUP_PROJECT()

# Search for dependencies.
SEARCH_FOR_BOOST()

# Libtool dynamic loading
# This project does not use Libtool directly but still uses ltdl for
# plug-in loading.
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(ltdl.h LTDL_H_FOUND)
IF (NOT LTDL_H_FOUND)
  MESSAGE(FATAL_ERROR
    "Failed to find ltdl.h, check that Libtool ltdl is installed.")
ENDIF()
#FIXME: check that libltdl.so is available.

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()
