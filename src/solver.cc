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
 * \file solver.cc
 *
 * \brief Implementation of the Solver class.
 */

#include <cassert>
#include "solver.hh"

namespace optimization
{
  Solver::Solver (function_t fct, size_type n, gradient_t gradient) throw ()
    : arity_ (n),
      function_ (fct),
      gradient_ (gradient),
      start_ (),
      constraints_ (),
      result_ (NoSolution ())
  {
  }

  Solver::~Solver () throw ()
  {
  }

  void
  Solver::reset () throw ()
  {
    result_ = NoSolution ();
  }

  void
  Solver::setStartingPoint (const array_t& point) throw ()
  {
    assert (point.size () == arity_);
    start_ = point;
    reset ();
  }

  Solver::function_t
  Solver::getFunction () const throw ()
  {
    return function_;
  }

  Solver::gradient_t
  Solver::getGradient () const throw ()
  {
    return gradient_;
  }

  std::size_t
  Solver::getArity () const throw ()
  {
    return arity_;
  }

  Solver::constraints_t&
  Solver::getConstraints () throw ()
  {
    return constraints_;
  }

  const Solver::constraints_t&
  Solver::getConstraints () const throw ()
  {
    return constraints_;
  }
} // end of namespace optimization
