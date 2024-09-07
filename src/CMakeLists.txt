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

SET(ROBOPTIM_CORE_SRC
  debug.hh
  doc.hh
  alloc.cc
  debug.cc
  finite-difference-gradient.cc
  generic-solver.cc
  indent.cc
  result.cc
  result-with-warnings.cc
  solver-error.cc
  solver-warning.cc
  solver.cc
  util.cc

  visualization/gnuplot.cc
  visualization/gnuplot-commands.cc
  visualization/gnuplot-differentiable-function.cc
  visualization/gnuplot-matrix.cc

  visualization/matplotlib.cc
  visualization/matplotlib-commands.cc
  visualization/matplotlib-matrix.cc
  )

IF(ROBOPTIM_PRECOMPILE_DENSE_SPARSE AND USE_CXX11)
  SET(ROBOPTIM_CORE_SRC ${ROBOPTIM_CORE_SRC} roboptim-core-precompile.cc)
ENDIF()

# Main library.
ADD_LIBRARY(roboptim-core SHARED ${HEADERS} ${ROBOPTIM_CORE_SRC})
TARGET_COMPILE_OPTIONS(roboptim-core PUBLIC "-Droboptim_core_EXPORTS")
IF(GENERATE_COVERAGE)
  TARGET_COMPILE_OPTIONS(roboptim-core PRIVATE "--coverage")
  SET_TARGET_PROPERTIES(roboptim-core PROPERTIES LINK_FLAGS "--coverage")
ENDIF()
target_include_directories(
  roboptim-core PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Add required libs to pkg-config file.
SET(ROBOPTIM_API_BOOST_LIBRARIES date_time system filesystem)
IF(NOT WIN32)
  PKG_CONFIG_APPEND_BOOST_LIBS(${ROBOPTIM_API_BOOST_LIBRARIES})
ELSE(NOT WIN32)
  # Only add the path
  PKG_CONFIG_APPEND_BOOST_LIBS()
ENDIF(NOT WIN32)

IF(ROBOPTIM_PRECOMPILE_DENSE_SPARSE)
  # core library needs to be linked with Boost libraries (e.g. for
  # OptimizationLogger)
  FOREACH(lib ${ROBOPTIM_API_BOOST_LIBRARIES})
    TARGET_LINK_LIBRARIES(roboptim-core boost_${lib})
  ENDFOREACH()
ENDIF()

IF(NOT LTDL_FOUND)
  TARGET_LINK_LIBRARIES(roboptim-core ltdl)
ELSE()
  PKG_CONFIG_USE_DEPENDENCY(roboptim-core ltdl)
ENDIF(NOT LTDL_FOUND)

SET_TARGET_PROPERTIES(roboptim-core PROPERTIES SOVERSION 3 VERSION 3.2.0)
INSTALL(TARGETS roboptim-core 
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
        )


# Dummy plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy MODULE dummy.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy PROPERTIES PREFIX "")

IF(NOT APPLE)
  SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy
    PROPERTIES VERSION 3.2.0 SOVERSION 3)
ENDIF()
INSTALL(TARGETS roboptim-core-plugin-dummy DESTINATION ${PLUGINDIR})

# Dummy-sparse plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy-sparse MODULE dummy-sparse.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy-sparse roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy-sparse roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-sparse PROPERTIES PREFIX "")

IF(NOT APPLE)
  SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-sparse
    PROPERTIES VERSION 3.2.0 SOVERSION 3)
ENDIF()
INSTALL(TARGETS roboptim-core-plugin-dummy-sparse DESTINATION ${PLUGINDIR})

# Dummy-laststate plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy-laststate MODULE dummy-laststate.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy-laststate roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy-laststate roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-laststate PROPERTIES PREFIX "")

IF(NOT APPLE)
  SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-laststate
    PROPERTIES VERSION 3.2.0 SOVERSION 3)
ENDIF()

# Dummy-laststate plug-in for differentiable sparse functions.
ADD_LIBRARY(roboptim-core-plugin-dummy-d-sparse-laststate MODULE dummy-d-sparse-laststate.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy-d-sparse-laststate roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy-d-sparse-laststate roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-d-sparse-laststate PROPERTIES PREFIX "")

IF(NOT APPLE)
  SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-d-sparse-laststate
    PROPERTIES VERSION 3.2.0 SOVERSION 3)
ENDIF()

# Dummy-td plug-in.
ADD_LIBRARY(roboptim-core-plugin-dummy-td MODULE dummy-td.cc)
ADD_DEPENDENCIES(roboptim-core-plugin-dummy-td roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-dummy-td roboptim-core)
SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-td PROPERTIES PREFIX "")

IF(NOT APPLE)
  SET_TARGET_PROPERTIES(roboptim-core-plugin-dummy-td
    PROPERTIES VERSION 3.2.0 SOVERSION 3)
ENDIF()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${TARGETS_EXPORT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

IF(MSVC)
  IF("${CMAKE_GENERATOR}" MATCHES "Visual Studio.*")
    SET(DEBUG_FOLDER "Debug")
  ENDIF("${CMAKE_GENERATOR}" MATCHES "Visual Studio.*")
  INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${DEBUG_FOLDER}/${PROJECT_NAME}${PROJECT_DEBUG_POSTFIX}.pdb" DESTINATION ${CMAKE_INSTALL_LIBDIR} CONFIGURATIONS Debug)
  INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${DEBUG_FOLDER}/" DESTINATION ${PLUGINDIR} CONFIGURATIONS Debug FILES_MATCHING PATTERN "${PROJECT_NAME}-plugin*.pdb")
ENDIF()
