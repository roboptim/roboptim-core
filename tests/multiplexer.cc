// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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
#include <roboptim/core/function.hh>
#include <roboptim/core/solver.hh>
#include <roboptim/core/problem.hh>
#include <roboptim/core/solver-factory.hh>
#include <roboptim/core/callback/multiplexer.hh>
#include <roboptim/core/optimization-logger.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Solver type.
typedef Solver<Function, boost::mpl::vector<Function> > solver_t;

// Multiplexer type.
typedef callback::Multiplexer<solver_t> multiplexer_t;

struct F : public Function
{
  typedef Function parent_t;

  F () : parent_t (4, 1, "a + b + c + d")
  {}

  void impl_compute (result_ref result, const_argument_ref x)
    const
  {
    result (0) = x[0] + x[1] + x[2] + x[3];
  }

  // No gradient, hessian.
};


void callbackFoo (const multiplexer_t::problem_t&,
                  multiplexer_t::solverState_t&)
{
  std::cout << "Foo" << std::endl;
}

void callbackBar (const multiplexer_t::problem_t&,
                  multiplexer_t::solverState_t&)
{
  std::cout << "Bar" << std::endl;
}

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (multiplexer)
{
  output = retrievePattern ("multiplexer");

  // Instantiate the function and the problem.
  F f;
  solver_t::problem_t pb (f);

  // Instantiate the factory using the dummy solver.
  SolverFactory<solver_t> factory ("dummy-laststate", pb);

  // Retrieve a solver instance from the factory.
  solver_t& solver = factory ();

  // Create a callback multiplexer.
  multiplexer_t multiplexer (solver);

  // Add some dummy callbacks.
  multiplexer.callbacks ().push_back (callbackFoo);
  multiplexer.callbacks ().push_back (callbackBar);

  // Add an optimization logger.
  boost::filesystem::path log_dir =
    "/tmp/roboptim-core-tests/multiplexer-logger";
  OptimizationLogger<solver_t> logger (solver, log_dir, false);
  multiplexer.callbacks ().push_back (logger.callback ());

  // Solve the problem.
  solver_t::result_t res = solver.minimum ();

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
