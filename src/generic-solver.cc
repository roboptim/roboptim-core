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

#include "debug.hh"

#include <cassert>
#include <log4cxx/logger.h>
#include "roboptim/core/generic-solver.hh"

namespace roboptim
{
  GenericSolver::GenericSolver ()
    : boost::noncopyable (),
      result_ (NoSolution ())
  {
  }

  GenericSolver::GenericSolver (const GenericSolver& solver)
    : boost::noncopyable (),
      result_ (solver.result_)
  {
  }

  GenericSolver::~GenericSolver ()
  {
  }

  void
  GenericSolver::reset ()
  {
    result_ = NoSolution ();
  }

  const GenericSolver::result_t&
  GenericSolver::minimum ()
  {
    if (result_.which () != SOLVER_NO_SOLUTION)
      return result_;
    solve ();
    assert (result_.which () != SOLVER_NO_SOLUTION);

    return result_;
  }

  std::ostream&
  GenericSolver::print (std::ostream& o) const
  {
    o << "Generic Solver";
    return o;
  }

  std::ostream&
  operator<< (std::ostream& o, const GenericSolver& gs)
  {
    return gs.print (o);
  }

  std::ostream&
  operator<< (std::ostream& o, const NoSolution&)
  {
    return o << "no solution";
  }
} // end of namespace roboptim
