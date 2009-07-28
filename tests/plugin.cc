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

#include <boost/mpl/vector.hpp>

#include "common.hh"

#include <roboptim/core/io.hh>
#include <roboptim/core/solver-factory.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<Function, boost::mpl::vector<Function> > solver_t;

// Define a simple function.
struct F : public Function
{
  F () : Function (4, 1, "a * d * (a + b + c) + d")
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
  // Instantiate the function and the problem.
  F f;
  solver_t::problem_t pb (f);

  // Instantiate the factory using the dummy solver.
  SolverFactory<solver_t> factory ("dummy", pb);

  // Retrieve a solver instance from the factory.
  solver_t& solver = factory ();

  // Get the Boost.Variant minimum.
  solver_t::result_t res = solver.minimum ();

  // Get the "real" minimum.
  solver.getMinimum<SolverError> ();

  // Display problem and solver.
  std::cout << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Try to get the minimum from a GenericSolver*.
  GenericSolver* gs = &solver;
  std::cout << gs->getMinimum <SolverError> ().what ()
            << std::endl;

  return 0;
}

GENERATE_TEST ()
