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
#include <roboptim/core/result-analyzer.hh>
#include <roboptim/core/scaling-helper.hh>
#include <roboptim/core/optimization-logger.hh>
#include <roboptim/core/callback/multiplexer.hh>
#include <roboptim/core/callback/wrapper.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/cos.hh>
#include <roboptim/core/function/identity.hh>
#include <roboptim/core/function/polynomial.hh>
#include <roboptim/core/function/sin.hh>

namespace roboptim
{
  // Disable GCC warnings for explicit template instantiations
  ROBOPTIM_ALLOW_ATTRIBUTES_ON

  template class GenericFunction<EigenMatrixDense>;
  template class GenericFunction<EigenMatrixSparse>;

  template class GenericDifferentiableFunction<EigenMatrixDense>;
  template class GenericDifferentiableFunction<EigenMatrixSparse>;

  template class GenericTwiceDifferentiableFunction<EigenMatrixDense>;
  template class GenericTwiceDifferentiableFunction<EigenMatrixSparse>;

  template class GenericLinearFunction<EigenMatrixDense>;
  template class GenericLinearFunction<EigenMatrixSparse>;

  template class GenericQuadraticFunction<EigenMatrixDense>;
  template class GenericQuadraticFunction<EigenMatrixSparse>;

  template class GenericNumericQuadraticFunction<EigenMatrixDense>;
  template class GenericNumericQuadraticFunction<EigenMatrixSparse>;

  template class GenericNumericLinearFunction<EigenMatrixDense>;
  template class GenericNumericLinearFunction<EigenMatrixSparse>;

  template class GenericSumOfC1Squares<EigenMatrixDense>;
  template class GenericSumOfC1Squares<EigenMatrixSparse>;

  template class Problem<EigenMatrixDense>;
  template class Problem<EigenMatrixSparse>;

  template class Solver<EigenMatrixDense>;
  template class Solver<EigenMatrixSparse>;

  template class SolverFactory<Solver<EigenMatrixDense> >;
  template class SolverFactory<Solver<EigenMatrixSparse> >;

  template class SolverState<Problem<EigenMatrixDense> >;
  template class SolverState<Problem<EigenMatrixSparse> >;

  template class SolverCallback<Solver<EigenMatrixDense> >;
  template class SolverCallback<Solver<EigenMatrixSparse> >;

  template class ResultAnalyzer<EigenMatrixDense>;
  template class ResultAnalyzer<EigenMatrixSparse>;

  template class ScalingHelper<EigenMatrixDense>;
  template class ScalingHelper<EigenMatrixSparse>;

  template class OptimizationLogger<Solver<EigenMatrixDense> >;
  template class OptimizationLogger<Solver<EigenMatrixSparse> >;

  template class GenericConstantFunction<EigenMatrixDense>;
  template class GenericConstantFunction<EigenMatrixSparse>;

  template class Cos<EigenMatrixDense>;
  template class Cos<EigenMatrixSparse>;

  template class GenericIdentityFunction<EigenMatrixDense>;
  template class GenericIdentityFunction<EigenMatrixSparse>;

  template class Polynomial<EigenMatrixDense>;
  template class Polynomial<EigenMatrixSparse>;

  template class Sin<EigenMatrixDense>;
  template class Sin<EigenMatrixSparse>;

  namespace callback
  {
    template class Multiplexer<Solver<EigenMatrixDense> >;
    template class Multiplexer<Solver<EigenMatrixSparse> >;

    template class Wrapper<Solver<EigenMatrixDense> >;
    template class Wrapper<Solver<EigenMatrixSparse> >;
  } // end of namespace callback

  ROBOPTIM_ALLOW_ATTRIBUTES_OFF
} // end of namespace roboptim
