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
 * \file src/solver.cc
 *
 * \brief Implementation of the Solver class.
 */

#include <cassert>
#include <algorithm>

#include "solver.hh"

namespace optimization
{
  LinearSolver::LinearSolver (const Function& f) throw ()
    : result_ (NoSolution ()),
      function_ (f)
  {
  }

  LinearSolver::~LinearSolver () throw ()
  {
  }

  void
  LinearSolver::reset () throw ()
  {
    result_ = NoSolution ();
  }

  const LinearSolver::function_t&
  LinearSolver::getFunction () const throw ()
  {
    return static_cast<const function_t&> (function_);
  }


  QuadraticSolver::QuadraticSolver (const Function& f) throw ()
    : LinearSolver (f)
  {
  }

  QuadraticSolver::~QuadraticSolver () throw ()
  {
  }

  const QuadraticSolver::function_t&
  QuadraticSolver::getFunction () const throw ()
  {
    return static_cast<const function_t&> (LinearSolver::getFunction ());
  }


  C2Solver::C2Solver (const Function& f) throw ()
    : QuadraticSolver (f)
  {
  }

  C2Solver::~C2Solver () throw ()
  {
  }

  const C2Solver::function_t&
  C2Solver::getFunction () const throw ()
  {
    return static_cast<const function_t&> (LinearSolver::getFunction ());
  }


  C1Solver::C1Solver (const Function& f) throw ()
    : C2Solver (f)
  {
  }

  C1Solver::~C1Solver () throw ()
  {
  }

  const C1Solver::function_t&
  C1Solver::getFunction () const throw ()
  {
    return static_cast<const function_t&> (LinearSolver::getFunction ());
  }


  C0Solver::C0Solver (const Function& f) throw ()
    : C1Solver (f)
  {
  }

  C0Solver::~C0Solver () throw ()
  {
  }

  const C0Solver::function_t&
  C0Solver::getFunction () const throw ()
  {
    return static_cast<const function_t&> (LinearSolver::getFunction ());
  }

} // end of namespace optimization
