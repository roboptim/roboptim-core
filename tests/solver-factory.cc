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

#include <boost/make_shared.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/plugin/dummy.hh>
#include <roboptim/core/twice-differentiable-function.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<EigenMatrixDense> solver_t;

// Output stream
boost::shared_ptr<boost::test_tools::output_test_stream> output;

struct F : public TwiceDifferentiableFunction
{
  F () : TwiceDifferentiableFunction (1, 1, "0")
  {
  }

  void
  impl_compute (result_ref, const_argument_ref) const
  {
  }

  void
  impl_gradient (gradient_ref, const_argument_ref, size_type) const
  {
  }

  void
  impl_hessian (hessian_ref, const_argument_ref, size_type) const
  {
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (solver_factory)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("solver-factory");

  // Create cost function.
  boost::shared_ptr<F> f = boost::make_shared<F> ();

  // Create problem.
  solver_t::problem_t pb (f);

  // Set bounds for all optimization parameters.
  for (std::size_t i = 0;
       static_cast<Function::size_type> (i) < pb.function ().inputSize (); ++i)
    pb.argumentBounds ()[i] = Function::makeInterval (0., 1.);

  // Set the starting point.
  Function::vector_t start (pb.function ().inputSize ());
  start.setZero ();

  // Initialize solver.
  SolverFactory<solver_t> factory ("dummy-td", pb);
  solver_t& solver = factory ();
  (*output) << solver << std::endl;

  BOOST_CHECK_THROW (SolverFactory<solver_t> factory_plugin ("dummy-foo", pb);
                     solver_t& solver_plugin = factory_plugin ();
                     (*output) << solver_plugin << std::endl,
                     std::runtime_error);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
