# Copyright 2013, Thomas Moulard, LAAS-CNRS
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

# Define the directory where plug-ins will be installed.
SET(PLUGINDIR lib/roboptim-core)

# Nag plug-in.
ADD_LIBRARY(roboptim-core-plugin-nag MODULE
  nag.cc ${HEADERS}
  )
PKG_CONFIG_USE_DEPENDENCY(roboptim-core-plugin-nag roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-nag nagc_nag)

SET_TARGET_PROPERTIES(roboptim-core-plugin-nag PROPERTIES
  PREFIX ""
  SOVERSION 1.1.0)

INSTALL(TARGETS roboptim-core-plugin-nag DESTINATION ${PLUGINDIR})

# Nag differentiable plug-in.
ADD_LIBRARY(roboptim-core-plugin-nag-differentiable MODULE
  nag.cc ${HEADERS}
  )
PKG_CONFIG_USE_DEPENDENCY(roboptim-core-plugin-nag-differentiable roboptim-core)
TARGET_LINK_LIBRARIES(roboptim-core-plugin-nag-differentiable nagc_nag)

SET_TARGET_PROPERTIES(roboptim-core-plugin-nag-differentiable PROPERTIES
  PREFIX ""
  SOVERSION 1.1.0)

INSTALL(TARGETS roboptim-core-plugin-nag-differentiable
  DESTINATION ${PLUGINDIR})