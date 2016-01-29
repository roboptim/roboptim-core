// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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
#include <boost/make_shared.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver.hh>
#include <roboptim/core/solver-error.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define a simple function.
struct F1 : public Function
{
  F1 () : Function (4, 1, "a + b + c + d")
  {}

  void impl_compute (result_ref res,
                     const_argument_ref x) const
  {
    res (0) = x[0] + x[1] + x[2] + x[3];
  }

  // No gradient, hessian.
};

class DummyErrorSolver : public Solver<EigenMatrixDense>
{
public:
  /// Define parent's type.
  typedef Solver<EigenMatrixDense> parent_t;
  typedef parent_t::solverState_t solverState_t;
  typedef parent_t::callback_t callback_t;

  DummyErrorSolver (const problem_t& pb)
    : parent_t (pb)
  {
  }

  ~DummyErrorSolver ()
  {
  }

  void solve ()
  {
    (*output) << "solve ()" << std::endl;

    result_ = SolverError ("The dummy solver always fail.\n"
                           "It also prints a dummy multiline error message.");
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (solver_error)
{
  output = retrievePattern ("solver-error");

  typedef DummyErrorSolver solver_t;
  boost::shared_ptr<F1> f = boost::make_shared<F1> ();
  solver_t::problem_t pb (f);

  boost::shared_ptr<solver_t> solver (new solver_t (pb));
  solver->solve ();

  (*output) << *solver << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
