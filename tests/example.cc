// Copyright (C) 2013 by Benjamin Chretien, CNRS.
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

#include <roboptim/core/io.hh>
#include <roboptim/core/plugin/dummy.hh>
#include <roboptim/core/twice-differentiable-function.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<TwiceDifferentiableFunction,
               boost::mpl::vector<TwiceDifferentiableFunction> > solver_t;

// Output stream
boost::shared_ptr<boost::test_tools::output_test_stream> output;


struct F : public TwiceDifferentiableFunction
{
  F () : TwiceDifferentiableFunction (4, 1, "x₀ * x₃ * (x₀ + x₁ + x₂) + x₃")
  {
  }

  void
  impl_compute (result_t& result, const argument_t& x) const
  {
    result (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  void
  impl_gradient (gradient_t& grad, const argument_t& x, size_type) const
  {
    grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
    grad[1] = x[0] * x[3];
    grad[2] = x[0] * x[3] + 1;
    grad[3] = x[0] * (x[0] + x[1] + x[2]);
  }

  void
  impl_hessian (hessian_t& h, const argument_t& x, size_type) const
  {
    h (0, 0) = 2 * x[3];
    h (0, 1) = x[3];
    h (0, 2) = x[3];
    h (0, 3) = 2 * x[0] + x[1] + x[2];

    h (1, 0) = x[3];
    h (1, 1) = 0.;
    h (1, 2) = 0.;
    h (1, 3) = x[0];

    h (2, 0) = x[3];
    h (2, 1) = 0.;
    h (2, 2) = 0.;
    h (2, 3) = x[1];

    h (3, 0) = 2 * x[0] + x[1] + x[2];
    h (3, 1) = x[0];
    h (3, 2) = x[0];
    h (3, 3) = 0.;
  }
};


struct G0 : public TwiceDifferentiableFunction
{
  G0 () : TwiceDifferentiableFunction (4, 1, "x₀ * x₁ * x₂ * x₃")
  {
  }

  void
  impl_compute (result_t& result, const argument_t& x) const
  {
    result (0) = x[0] * x[1] * x[2] * x[3];
  }

  void
  impl_gradient (gradient_t& grad, const argument_t& x, size_type) const
  {
    grad[0] = x[1] * x[2] * x[3];
    grad[1] = x[0] * x[2] * x[3];
    grad[2] = x[0] * x[1] * x[3];
    grad[3] = x[0] * x[1] * x[2];
  }

  void
  impl_hessian (hessian_t& h, const argument_t& x, size_type) const
  {
    h (0, 0) = 0.;
    h (0, 1) = x[2] * x[3];
    h (0, 2) = x[1] * x[3];
    h (0, 3) = x[1] * x[2];

    h (1, 0) = x[2] * x[3];
    h (1, 1) = 0.;
    h (1, 2) = x[0] * x[3];
    h (1, 3) = x[0] * x[2];

    h (2, 0) = x[1] * x[3];
    h (2, 1) = x[0] * x[3];
    h (2, 2) = 0.;
    h (2, 3) = x[0] * x[1];

    h (3, 0) = x[1] * x[2];
    h (3, 1) = x[0] * x[2];
    h (3, 2) = x[0] * x[1];
    h (3, 3) = 0.;
  }
};

struct G1 : public TwiceDifferentiableFunction
{
  G1 () : TwiceDifferentiableFunction (4, 1, "x₀² + x₁² + x₂² + x₃²")
  {
  }

  void
  impl_compute (result_t& result, const argument_t& x) const
  {
    result (0) = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
  }

  void
  impl_gradient (gradient_t& grad, const argument_t& x, size_type) const
  {
    grad[0] = 2 * x[0];
    grad[1] = 2 * x[1];
    grad[2] = 2 * x[2];
    grad[3] = 2 * x[3];
  }

  void
  impl_hessian (hessian_t& h, const argument_t& /*x*/, size_type) const
  {
    h (0, 0) = 2.;
    h (0, 1) = 0.;
    h (0, 2) = 0.;
    h (0, 3) = 0.;

    h (1, 0) = 0.;
    h (1, 1) = 2.;
    h (1, 2) = 0.;
    h (1, 3) = 0.;

    h (2, 0) = 0.;
    h (2, 1) = 0.;
    h (2, 2) = 2.;
    h (2, 3) = 0.;

    h (3, 0) = 0.;
    h (3, 1) = 0.;
    h (3, 2) = 0.;
    h (3, 3) = 2.;
  }
};


int run_test (boost::shared_ptr<boost::test_tools::output_test_stream>& output)
{
  // Create cost function.
  F f;

  // Create problem.
  solver_t::problem_t pb (f);

  // Set bounds for all optimization parameters.
  // 1. < x_i < 5. (x_i in [1.;5.])
  for (std::size_t i = 0;
       static_cast<Function::size_type> (i) < pb.function ().inputSize (); ++i)
    pb.argumentBounds ()[i] = Function::makeInterval (1., 5.);

  // Set the starting point.
  Function::vector_t start (pb.function ().inputSize ());
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;

  // Create constraints.
  boost::shared_ptr<G0> g0 (new G0 ());
  boost::shared_ptr<G1> g1 (new G1 ());

  F::intervals_t bounds;
  solver_t::problem_t::scales_t scales;

  // Add constraints
  bounds.push_back(Function::makeLowerInterval (25.));
  scales.push_back (1.);
  pb.addConstraint
    (boost::static_pointer_cast<TwiceDifferentiableFunction> (g0),
     bounds, scales);

  bounds.clear ();
  scales.clear ();

  bounds.push_back(Function::makeInterval (40., 40.));
  scales.push_back (1.);
  pb.addConstraint
    (boost::static_pointer_cast<TwiceDifferentiableFunction> (g1),
     bounds, scales);

  // Initialize solver.

  // Here we are relying on a dummy solver.
  // You may change this string to load the solver you wish to use:
  //  - Ipopt: "ipopt", "ipopt-sparse", "ipopt-td"
  //  - Eigen: "eigen-levenberg-marquardt"
  //  etc.
  // The plugin is built for a given solver type, so chose it adequately.
  SolverFactory<solver_t> factory ("dummy-td", pb);
  solver_t& solver = factory ();

  // Compute the minimum and retrieve the result.
  solver_t::result_t res = solver.minimum ();

  // Display solver information.
  (*output) << solver << std::endl;

  // Check if the minimization has succeeded.

  // Process the result
  switch (res.which ())
    {
    case solver_t::SOLVER_VALUE:
      {
        // Get the result.
        Result& result = boost::get<Result> (res);

        // Display the result.
        (*output) << "A solution has been found: " << std::endl
                  << result << std::endl;

        break;
      }

    case solver_t::SOLVER_VALUE_WARNINGS:
      {
        // Get the result.
        ResultWithWarnings& result = boost::get<ResultWithWarnings> (res);

        // Display the result.
        (*output) << "A solution has been found: " << std::endl
                  << result << std::endl;

        break;
      }

    case solver_t::SOLVER_NO_SOLUTION:
    case solver_t::SOLVER_ERROR:
      {
        (*output) << "A solution has not been found. As expected..."
                  << std::endl
                  << boost::get<SolverError> (res).what ()
                  << std::endl;

        return solver_t::SOLVER_NO_SOLUTION;
      }
    }

  return res.which ();
}


BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (example)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("example");

  BOOST_CHECK_EQUAL (run_test (output), solver_t::SOLVER_NO_SOLUTION);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
