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

#include "shared-tests/fixture.hh"

#include <iostream>

#include <boost/mpl/vector.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver-factory.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<EigenMatrixDense> solver_t;

// Define a simple function.
struct F : public Function
{
  F () : Function (4, 1, "a * d * (a + b + c) + d")
  {}

  void impl_compute (result_ref result, const_argument_ref argument)
    const
  {
    result (0) = argument[0] * argument[3]
      * (argument[0] + argument[1] + argument[2]) + argument[3];
  }

  // No gradient, hessian.
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (plugin)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("plugin-laststate");

  // Instantiate the function and the problem.
  F f;
  solver_t::problem_t pb (f);

  // Instantiate the factory using the dummy solver.
  SolverFactory<solver_t> factory ("dummy-laststate", pb);

  // Retrieve a solver instance from the factory.
  solver_t& solver = factory ();

  // Get the Boost.Variant minimum.
  solver_t::result_t res = solver.minimum ();

  // Get the "real" minimum.
  solver.getMinimum<SolverError> ();

  // Display problem and solver.
  (*output) << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Display the solver's last state (x, objective etc.)
  SolverError& err = boost::get<SolverError> (res);
  const boost::optional<Result>& lastState = err.lastState();
  if(lastState)
  {
    (*output) << "Last dummy x: " << lastState->x
              << std::endl;
    (*output) << "Last dummy value: " << lastState->value
              << std::endl << std::endl;
  }

  // Try to get the minimum from a GenericSolver*.
  GenericSolver* gs = &solver;
  (*output) << gs->getMinimum <SolverError> ().what ()
            << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
