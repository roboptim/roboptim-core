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

#include <boost/make_shared.hpp>

#include <roboptim/core.hh>

#include <roboptim/core/plugin/dummy.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef DummySolver solver_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define a simple function.
struct F : public Function
{
  F () : Function (4, 1, "a * d * (a + b + c) + d")
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = argument[0] * argument[3]
      * (argument[0] + argument[1] + argument[2]) + argument[3];
  }

  // No gradient, hessian.
};


struct CountUnaryFunction
{
  typedef void Result;

  CountUnaryFunction (int& cnt)
    : cnt_ (cnt)
  {}

  void operator () (double x)
  {
    (*output) << "Discrete point: " << x
	      << " (cnt: " << ++cnt_ << ")"
	      << std::endl;
  }

private:
  int& cnt_;
};


BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (simple)
{
  output = retrievePattern ("simple");

  // Instantiate the function and the problem.
  boost::shared_ptr<F> f = boost::make_shared<F> ();
  DummySolver::problem_t pb (f);

  (*output) << pb << std::endl;

  // Check that the problem is well formed.
  BOOST_CHECK_EQUAL (&pb.function (), f.get ());
  BOOST_CHECK_EQUAL (pb.constraints ().size (), 0u);
  BOOST_CHECK (!pb.startingPoint ());

  Function::vector_t x (4);
  x.setZero ();
  pb.startingPoint () = x;
  BOOST_CHECK_EQUAL (pb.startingPoint ()->size (), 4);

  BOOST_CHECK_EQUAL (pb.boundsVector ().size (), 0u);
  BOOST_CHECK_EQUAL (pb.argumentBounds ().size (), 4u);
  BOOST_CHECK
    (pb.argumentBounds ()[0] == Function::makeInfiniteInterval ()
     && pb.argumentBounds ()[1] == Function::makeInfiniteInterval ()
     && pb.argumentBounds ()[2] == Function::makeInfiniteInterval ()
     && pb.argumentBounds ()[3] == Function::makeInfiniteInterval ());


  BOOST_CHECK_EQUAL (pb.scalingVector ().size (), 0u);
  BOOST_CHECK_EQUAL (pb.argumentScaling ().size (), 4u);
  BOOST_CHECK
    (pb.argumentScaling ()[0] == 1.
     && pb.argumentScaling ()[1] == 1.
     && pb.argumentScaling ()[2] == 1.
     && pb.argumentScaling ()[3] == 1.);

  F* g = new F ();
  pb.addConstraint (boost::shared_ptr<F> (g),
  		    Function::makeInterval (0., 5.), 3.5);
  BOOST_CHECK_EQUAL (pb.constraints ().size (), 1u);
  BOOST_CHECK_EQUAL (pb.boundsVector ().size (), 1u);
  BOOST_CHECK_EQUAL (pb.boundsVector ()[0][0].first,
		     0.);
  BOOST_CHECK_EQUAL (pb.boundsVector ()[0][0].second,
		     5.);
  BOOST_CHECK_EQUAL (pb.scalingVector ().size (), 1u);
  BOOST_CHECK_EQUAL (pb.scalingVector ()[0][0], 3.5);


  // Try to solve it with the DummySolver (direct instantiation, no plug-in).
  solver_t solver (pb);
  solver_t::result_t res = solver.minimum ();
  solver.getMinimum<SolverError> ();

  (*output) << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Try to get the minimum from a GenericSolver*.
  GenericSolver* gs = &solver;
  (*output) << gs->getMinimum<SolverError> ().what ()
            << std::endl;

  // Check iteration in discrete intervals.
  {
    int cnt = 0;
    CountUnaryFunction count (cnt);

    {
      Function::discreteInterval_t interval (2.3, 3., 0.5);
      Function::foreach (interval, count);
      BOOST_CHECK_EQUAL (cnt, 2);
      (*output) << std::endl;
    }

    {
      cnt = 0;
      Function::discreteInterval_t interval (2., 3., 0.1);
      Function::foreach (interval, count);
      BOOST_CHECK_EQUAL (cnt, 11);
      (*output) << std::endl;
    }

    {
      cnt = 0;
      Function::discreteInterval_t interval (0.8, 10.8, 1.);
      Function::foreach (interval, count);
      BOOST_CHECK_EQUAL (cnt, 11);
      (*output) << std::endl;
    }
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
