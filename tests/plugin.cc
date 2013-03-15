// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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
#include <boost/mpl/vector.hpp>

#include <roboptim/core/solver-factory.hh>

#include "shared-tests/common.hh"

using namespace roboptim;
typedef Solver<Function, boost::mpl::vector<> > solver_t;

struct Square : public DifferentiableFunction
{
  Square () : DifferentiableFunction (1, 1, "x * x")
  {
  }

  void
  impl_compute (result_t& result, const argument_t& x) const throw ()
  {
    result.setZero ();
    result (0) = x[0] * x[0];
  }

  void
  impl_gradient (gradient_t& grad, const argument_t&, size_type) const throw ()
  {
    grad[0] = 2;
  }
};


void testme (const std::string& solverName,
	     const std::string& pattern)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern (pattern);

  Square f;
  solver_t::problem_t pb (f);

  // Set bound for all variables.
  // 1. < x_i < 5. (x_i in [1.;5.])
  for (std::size_t i = 0;
       i < static_cast<std::size_t> (pb.function ().inputSize ()); ++i)
    pb.argumentBounds ()[i] = Function::makeInterval (1., 5.);

  // Set the starting point.
  Function::vector_t start (pb.function ().inputSize ());
  start[0] = 4.;
  pb.startingPoint () = start;

  // Initialize solver
  SolverFactory<solver_t> factory (solverName, pb);
  solver_t& solver = factory ();

  // Compute the minimum and retrieve the result.
  solver_t::result_t res = solver.minimum ();

  // Display solver information.
  (*output) << solver << std::endl;

  // Check if the minimization has succeed.
  if (res.which () != solver_t::SOLVER_VALUE)
    {
      std::cout << "A solution should have been found. Failing..."
                << std::endl
                << boost::get<SolverError> (res).what ()
                << std::endl;
      BOOST_CHECK_EQUAL (res.which (), solver_t::SOLVER_VALUE);
    }

  // Get the result.
  Result& result = boost::get<Result> (res);

  // Display the result.
  (*output) << "A solution has been found: " << std::endl;
  (*output) << result.x << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_CASE (simple)
{
  testme ("nag", "plugin-simple");
}

BOOST_AUTO_TEST_CASE (differentiable)
{
  testme ("nag-differentiable", "plugin-simple");
}
