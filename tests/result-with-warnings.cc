// Copyright (C) 2014 by Benjamin Chrétien, CNRS.
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

#include <boost/shared_ptr.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver.hh>
#include <roboptim/core/solver-warning.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<Function, boost::mpl::vector<Function> > parent_solver_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define a simple function.
struct F : public Function
{
  F () : Function (1, 1, "x")
  {}

  void impl_compute (result_t& res,
                     const argument_t& x) const
  {
    res (0) = x[0];
  }

  // No gradient, hessian.
};

class DummyWarningSolver : public parent_solver_t
{
public:
  /// Define parent's type.
  typedef parent_solver_t parent_t;
  typedef Function::argument_t argument_t;

  DummyWarningSolver (const problem_t& pb)
    : parent_t (pb)
  {
  }

  ~DummyWarningSolver ()
  {
  }

  void solve ()
  {
    (*output) << "solve ()" << std::endl;

    argument_t x (problem ().function ().inputSize ());
    x.setZero ();

    ResultWithWarnings result (problem ().function ().inputSize (),
			       problem ().function ().outputSize ());
    result.x = x;
    result.value = problem ().function () (result.x);
    result.warnings.push_back (SolverWarning ("Dummy warning message."));

    result_ = result;
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (result_with_warnings)
{
  typedef DummyWarningSolver solver_t;

  output = retrievePattern ("result-with-warnings");

  // Instantiate the function, the problem, and solve it.
  F f;
  solver_t::problem_t pb (f);
  boost::shared_ptr<solver_t> solver (new solver_t (pb));
  solver_t::result_t result = solver->minimum ();

  // Get the result.
  ResultWithWarnings& result_warnings = boost::get<ResultWithWarnings> (result);
  (*output) << result_warnings << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
