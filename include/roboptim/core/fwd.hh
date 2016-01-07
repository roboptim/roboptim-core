// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_FWD_HH
# define ROBOPTIM_CORE_FWD_HH

// TODO: remove as soon as the typeString() problem is solved
# include <iosfwd>

# include <roboptim/core/portability.hh>

namespace roboptim
{
  /// \brief Tag a result if no solution has been found.
  ///
  /// Returned by GenericSolver::getMinimum if no solution can be
  /// found (but no error has been encountered during the process).
  /// It usually means that the solver has not been called to solve
  /// the problem.
  class NoSolution {};

  class DummySolver;

  namespace finiteDifferenceGradientPolicies
  {
    template <typename T>
    class Simple;
    template <typename T>
    class FivePointsRule;
  } // end of finiteDifferenceGradientPolicies

  template <typename T,
	    typename FdgPolicy =
	    finiteDifferenceGradientPolicies::FivePointsRule<T> >
  class GenericFiniteDifferenceGradient;

  template <typename T>
  struct GenericFunctionTraits;

  /// \brief Tag type for functions using Eigen dense matrices.
  struct ROBOPTIM_DLLAPI EigenMatrixDense {};
  /// \brief Tag type for functions using Eigen sparse matrices.
  struct ROBOPTIM_DLLAPI EigenMatrixSparse {};

  template <typename T>
  class GenericFunction;

  /// \brief Dense function.
  typedef GenericFunction<EigenMatrixDense>
  Function;

  /// \brief Sparse function.
  typedef GenericFunction<EigenMatrixSparse>
  SparseFunction;

  template <typename T>
  class GenericDifferentiableFunction;

  /// \brief Dense differentiable function.
  typedef GenericDifferentiableFunction<EigenMatrixDense>
  DifferentiableFunction;

  /// \brief Sparse differentiable function.
  typedef GenericDifferentiableFunction<EigenMatrixSparse>
  DifferentiableSparseFunction;

  template <typename T>
  class GenericConstantFunction;

  template <typename U, typename V>
  class Minus;
  template <typename U, typename V>
  class Plus;
  template <typename U, typename V>
  class Product;
  template <typename U>
  class Scalar;


  class GenericSolver;
  class NoSolution;

  template <typename T>
  class GenericNumericLinearFunction;
  typedef GenericNumericLinearFunction<EigenMatrixDense>
  NumericLinearFunction;
  typedef GenericNumericLinearFunction<EigenMatrixSparse>
  NumericLinearSparseFunction;

  template <typename T>
  class GenericNumericQuadraticFunction;
  typedef GenericNumericQuadraticFunction<EigenMatrixDense>
  NumericQuadraticFunction;
  typedef GenericNumericQuadraticFunction<EigenMatrixSparse>
  NumericQuadraticSparseFunction;

  template <typename T>
  class GenericConstantFunction;
  typedef GenericConstantFunction<EigenMatrixDense> ConstantFunction;

  template <typename T>
  class GenericIdentityFunction;
  typedef GenericIdentityFunction<EigenMatrixDense> IdentityFunction;

  template <typename T>
  class Cos;
  template <typename T>
  class Sin;



  class Result;
  class ResultWithWarnings;
  class SolverError;
  class SolverWarning;

  template <typename T>
  class GenericTwiceDifferentiableFunction;
  typedef GenericTwiceDifferentiableFunction<EigenMatrixDense>
  TwiceDifferentiableFunction;
  typedef GenericTwiceDifferentiableFunction<EigenMatrixSparse>
  TwiceDifferentiableSparseFunction;

  template <typename T>
  class GenericLinearFunction;
  typedef GenericLinearFunction<EigenMatrixDense> LinearFunction;
  typedef GenericLinearFunction<EigenMatrixSparse> LinearSparseFunction;

  template <typename T>
  class GenericQuadraticFunction;
  typedef GenericQuadraticFunction<EigenMatrixDense> QuadraticFunction;
  typedef GenericQuadraticFunction<EigenMatrixSparse> QuadraticSparseFunction;

  template <typename T> class Problem;
  template <typename T> class Solver;
  template <typename S> class SolverFactory;
  template <unsigned DerivabilityOrder> class NTimesDerivableFunction;

  template <typename T>
  struct derivativeSize;

  template <typename K, typename V, typename H> class LRUCache;

  template <typename T>
  class OptimizationLogger;

  // TODO: remove, this is only here because of an unfortunate circular
  // dependency between function.hh and util.hh
  template <typename T>
  std::string typeString ();
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FWD_HH
