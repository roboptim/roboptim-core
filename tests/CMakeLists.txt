# Copyright 2013, Thomas Moulard, CNRS-AIST.
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

# Include shared test suite.
SET(PLUGIN_PATH "${CMAKE_BINARY_DIR}/src")

#FIXME: no problem whose input size is 1 in the test suite for now.

#SET(SOLVER_NAME "nag")
#SET(SCHITTKOWSKI_PROBLEMS 1 2 3)
#SET(FUNCTION_TYPE ::roboptim::EigenMatrixDense)
#SET(COST_FUNCTION_TYPE ::roboptim::GenericFunction)
#SET(CONSTRAINT_TYPE_1 False)
#SET(CONSTRAINT_TYPE_2 False)
#SET(PROGRAM_SUFFIX "")
#INCLUDE(shared-tests/CMakeLists.txt)

#SET(SOLVER_NAME "nag-differentiable")
#SET(SCHITTKOWSKI_PROBLEMS 1 2 3 4)
#SET(FUNCTION_TYPE ::roboptim::EigenMatrixDense)
#SET(COST_FUNCTION_TYPE ::roboptim::GenericDifferentiableFunction)
#SET(CONSTRAINT_TYPE_1 False)
#SET(CONSTRAINT_TYPE_2 False)
#SET(PROGRAM_SUFFIX "-differentiable")
#INCLUDE(shared-tests/CMakeLists.txt)

#SET(SOLVER_NAME "nag-simplex")
#SET(SCHITTKOWSKI_PROBLEMS 1 2 3 4)
#SET(FUNCTION_TYPE ::roboptim::EigenMatrixDense)
#SET(COST_FUNCTION_TYPE ::roboptim::GenericDifferentiableFunction)
#SET(CONSTRAINT_TYPE_1 False)
#SET(CONSTRAINT_TYPE_2 False)
#SET(PROGRAM_SUFFIX "-simplex")
#INCLUDE(shared-tests/CMakeLists.txt)

SET(SOLVER_NAME "nag-nlp")
SET(FUNCTION_TYPE ::roboptim::EigenMatrixDense)
SET(PROGRAM_SUFFIX "-nlp")
SET(COST_FUNCTION_TYPE ::roboptim::GenericDifferentiableFunction)
SET(CONSTRAINT_TYPE_1 ::roboptim::GenericLinearFunction)
SET(CONSTRAINT_TYPE_2 ::roboptim::GenericDifferentiableFunction)
INCLUDE(shared-tests/CMakeLists.txt)

SET(SOLVER_NAME "nag-nlp-sparse")
SET(FUNCTION_TYPE ::roboptim::EigenMatrixSparse)
SET(PROGRAM_SUFFIX "-nlp-sparse")
SET(COST_FUNCTION_TYPE ::roboptim::GenericDifferentiableFunction)
SET(CONSTRAINT_TYPE_1 ::roboptim::GenericLinearFunction)
SET(CONSTRAINT_TYPE_2 ::roboptim::GenericDifferentiableFunction)
INCLUDE(shared-tests/CMakeLists.txt)
