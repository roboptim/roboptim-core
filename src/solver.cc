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
  Solver::Solver (function_t fct,
                  size_type n,
                  gradient_t gradient,
                  hessian_t hessian,
                  jacobian_t jacobian) throw ()
    : arity_ (n),
      function_ (fct),
      gradient_ (gradient),
      start_ (),
      bounds_ (n),
      constraints_ (),
      hessian_ (hessian),
      jacobian_ (jacobian),
      result_ (NoSolution ())
  {
    std::fill (bounds_.begin (), bounds_.end (),
               std::make_pair (-std::numeric_limits<value_type>::infinity (),
                               std::numeric_limits<value_type>::infinity ()));
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

  Solver::hessian_t
  Solver::getHessian () const throw ()
  {
    return hessian_;
  }

  Solver::jacobian_t
  Solver::getJacobian () const throw ()
  {
    return jacobian_;
  }

  Solver::size_type
  Solver::getArity () const throw ()
  {
    return arity_;
  }

  Solver::bound_t
  Solver::getBound(size_type i) const throw ()
  {
    assert (i < getArity ());
    return bounds_[i];
  }

  void
  Solver::setBound(size_type i, bound_t b) throw ()
  {
    assert (i < getArity ());
    bounds_[i] = b;
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
