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

/**
 * \brief Forward declarations.
 */

#ifndef ROBOPTIM_CORE_FWD_HH
# define ROBOPTIM_CORE_FWD_HH

namespace roboptim
{
  /// \brief Returned by GenericSolver::getMinimum if no solution can be
  /// found (but no error has been encountered during the process).
  class NoSolution {};

  class Function;
  class DerivableFunction;
  class TwiceDerivableFunction;
  class QuadraticFunction;
  class LinearFunction;

  template <typename F, typename C = F>
  class Problem;

  class GenericSolver;
  template <typename F, typename C = F>
  class Solver;

  class CFSQPSolver;
  class DummySolver;
  class IpoptSolver;

  class Result;
  class ResultWithWarnings;

  class SolverError;
  class SolverWarning;
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FWD_HH
