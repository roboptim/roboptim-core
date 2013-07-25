# Copyright 2013, Thomas Moulard, CNRS JRL UMI3218
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
INCLUDE(cmake/cpack.cmake)

SET(PROJECT_NAME roboptim-core-plugin-nag)
SET(PROJECT_DESCRIPTION "RobOptim core NAG plug-in")
SET(PROJECT_URL "http://github.com/laas/roboptim-core-plugin-nag")

SET(HEADERS
  )

SET(PKG_CONFIG_ADDITIONAL_VARIABLES plugindir ${PKG_CONFIG_ADDITIONAL_VARIABLES})

SETUP_PROJECT()

# If compiler support symbol visibility, enable it.
INCLUDE(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAS_VISIBILITY)
IF(HAS_VISIBILITY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
ENDIF()

# Search for dependencies.
SEARCH_FOR_BOOST()

# Search for Ipopt.
ADD_REQUIRED_DEPENDENCY("roboptim-core >= 0.5")

# Look for NAG.
FIND_PATH(NAG_DIR include/nag.h HINTS /opt/NAG/cll6a23dhl/)

INCLUDE_DIRECTORIES("${NAG_DIR}/include")
LINK_DIRECTORIES("${NAG_DIR}/lib")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

SETUP_PROJECT_FINALIZE()
