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
 * \file src/dummy.cc
 *
 * \brief Implementation of the dummy module (always fail).
 */

#include "dummy.hh"

namespace optimization
{
  DummySolver::DummySolver (const Function& f) throw ()
    : C0Solver (f)
  {
    result_ = SolverError ("The dummy solver always fail.");
  }

  DummySolver::~DummySolver () throw ()
  {
  }

  DummySolver::result_t
  DummySolver::getMinimum () throw ()
  {
    return result_;
  }

  void
  DummySolver::addLinearConstraint (const LinearFunction&) throw ()
  {
  }

  void
  DummySolver::addQuadraticConstraint (const QuadraticFunction&) throw ()
  {
  }

  void
  DummySolver::addC2Constraint (const TwiceDerivableFunction&) throw ()
  {
  }

  void
  DummySolver::addC1Constraint (const DerivableFunction&) throw ()
  {
  }

  void
  DummySolver::addC0Constraint (const Function&) throw ()
  {
  }

} // end of namespace optimization
