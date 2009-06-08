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

#include <iostream>

#include "common.hh"
#include <roboptim/core/solver-factory.hh>

using namespace roboptim;

typedef Solver<Function, const Function*> solver_t;

struct F : public Function
{
  F () : Function (4, 1)
  {}

  void impl_compute (result_t& result, const argument_t& argument) const throw ()
  {
    result (0) = argument[0] * argument[3]
      * (argument[0] + argument[1] + argument[2]) + argument[3];
  }

  // No gradient, hessian.
};

int run_test ()
{
  F f;
  solver_t::problem_t pb (f);

  SolverFactory<solver_t> factory ("dummy", pb);
  solver_t& solver = factory ();
  solver_t::result_t res = solver.minimum ();
  solver.getMinimum<SolverError> ();

  std::cout << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Try to get the minimum from a GenericSolver*
  GenericSolver* gs = &solver;
  std::cout << gs->getMinimum <SolverError> ().what ()
            << std::endl;

  return 0;
}

GENERATE_TEST ()
