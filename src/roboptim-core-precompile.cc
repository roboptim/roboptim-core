// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#include <roboptim/core/portability.hh>
#include <roboptim/core/function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/sum-of-c1-squares.hh>
#include <roboptim/core/problem.hh>
#include <roboptim/core/solver.hh>
#include <roboptim/core/solver-callback.hh>
#include <roboptim/core/solver-factory.hh>
#include <roboptim/core/optimization-logger.hh>
#include <roboptim/core/callback/multiplexer.hh>
#include <roboptim/core/callback/wrapper.hh>

namespace roboptim
{
  // Disable GCC warnings for explicit template instantiations
  ROBOPTIM_ALLOW_ATTRIBUTES_ON

  template class ROBOPTIM_CORE_DLLAPI GenericFunction<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI GenericFunction<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI GenericNumericQuadraticFunction<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI GenericNumericQuadraticFunction<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI
    GenericNumericLinearFunction<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI
    GenericNumericLinearFunction<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI GenericSumOfC1Squares<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI GenericSumOfC1Squares<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI Problem<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI Problem<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI Solver<EigenMatrixDense>;
  template class ROBOPTIM_CORE_DLLAPI Solver<EigenMatrixSparse>;

  template class ROBOPTIM_CORE_DLLAPI SolverFactory<Solver<EigenMatrixDense> >;
  template class ROBOPTIM_CORE_DLLAPI SolverFactory<Solver<EigenMatrixSparse> >;

  template class ROBOPTIM_CORE_DLLAPI SolverState<Problem<EigenMatrixDense> >;
  template class ROBOPTIM_CORE_DLLAPI SolverState<Problem<EigenMatrixSparse> >;

  template class ROBOPTIM_CORE_DLLAPI SolverCallback<Solver<EigenMatrixDense> >;
  template class ROBOPTIM_CORE_DLLAPI SolverCallback<Solver<EigenMatrixSparse> >;

  template class ROBOPTIM_CORE_DLLAPI
    OptimizationLogger<Solver<EigenMatrixDense> >;
  template class ROBOPTIM_CORE_DLLAPI
    OptimizationLogger<Solver<EigenMatrixSparse> >;

  namespace callback
  {
    template class ROBOPTIM_CORE_DLLAPI Multiplexer<Solver<EigenMatrixDense> >;
    template class ROBOPTIM_CORE_DLLAPI Multiplexer<Solver<EigenMatrixSparse> >;

    template class ROBOPTIM_CORE_DLLAPI Wrapper<Solver<EigenMatrixDense> >;
    template class ROBOPTIM_CORE_DLLAPI Wrapper<Solver<EigenMatrixSparse> >;
  } // end of namespace callback

  ROBOPTIM_ALLOW_ATTRIBUTES_OFF
} // end of namespace roboptim
