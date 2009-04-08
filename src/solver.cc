// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \brief Implementation of the Solver class.
 */

#include <cassert>
#include "liboptimization/solver.hh"

namespace optimization
{
  GenericSolver::GenericSolver () throw ()
    : result_ (NoSolution ())
  {
  }

  GenericSolver::~GenericSolver () throw ()
  {
  }

  void
  GenericSolver::reset () throw ()
  {
    result_ = NoSolution ();
  }

  const GenericSolver::result_t&
  GenericSolver::getMinimum () throw ()
  {
    if (result_.which () != SOLVER_NO_SOLUTION)
      return result_;
    solve ();
    assert (result_.which () != SOLVER_NO_SOLUTION);
    return result_;
  }

} // end of namespace optimization
