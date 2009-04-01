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
  Solver::Solver (const Problem& pb) throw ()
    : problem (pb),
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
} // end of namespace optimization
