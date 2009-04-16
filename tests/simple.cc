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
#include <boost/variant/get.hpp>

#include "common.hh"
#include <roboptim-core/dummy.hh>

using namespace roboptim;

typedef DummySolver solver_t;

struct F : public Function
{
  F () : Function (4, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
    return res;
  }

  // No gradient, hessian.
};

int run_test ()
{
  F f;
  DummySolver::problem_t pb (f);

  solver_t solver (pb);
  solver_t::result_t res = solver.minimum ();
  boost::get<SolverError> (res);

  std::cout << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Try to get the minimum from a GenericSolver*
  GenericSolver* gs = &solver;
  std::cout << boost::get<SolverError> (gs->minimum ()).what ()
            << std::endl;

  return 0;
}

GENERATE_TEST ()
