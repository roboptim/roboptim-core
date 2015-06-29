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
#include <boost/filesystem.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver-factory.hh>
#include <roboptim/core/optimization-logger.hh>
#include <roboptim/core/differentiable-function.hh>

using namespace roboptim;

// Define a simple function (non-differentiable)
struct F1 : public Function
{
  typedef Function parent_t;

  F1 () : parent_t (4, 1, "a * d * (a + b + c) + d")
  {}

  void impl_compute (result_ref result, const_argument_ref x)
    const
  {
    result (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  // No gradient, hessian.
};

// Define a simple function (sparse differentiable)
struct F2 : public DifferentiableSparseFunction
{
  typedef DifferentiableSparseFunction parent_t;

  F2 () : parent_t (4, 1, "a + b + c + d")
  {}

  void impl_compute (result_ref result, const_argument_ref x)
    const
  {
    result (0) = x[0] + x[1] + x[2] + x[3];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref, size_type)
    const
  {
    grad.insert (0) = 1;
    grad.insert (1) = 1;
    grad.insert (2) = 1;
    grad.insert (3) = 1;
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

// See: http://stackoverflow.com/a/596348/1043187
template <typename>
struct toString
{
  static char const* value () { return "unknown"; }
};

#define DEF_FUNC_TYPE(X)			\
  template <>					\
  struct toString<X>				\
  {						\
    static char const* value () { return #X; }	\
  }

DEF_FUNC_TYPE(F1);
DEF_FUNC_TYPE(F2);

template <>
struct toString<Function>
{
  static char const* value () { return ""; }
};

template <>
struct toString<DifferentiableSparseFunction>
{
  static char const* value () { return "-d-sparse"; }
};

template <typename F>
void testLogger
(boost::shared_ptr<boost::test_tools::output_test_stream> output)
{
  // Specify the solver that will be used.
  typedef Solver<typename F::traits_t> solver_t;

  // Instantiate the function and the problem.
  F f;
  typename solver_t::problem_t pb (f);

  // Add a dummy constraint to test logging
  boost::shared_ptr<F> g = boost::make_shared<F> ();
  pb.addConstraint (g, F::makeInterval (-1., 1.));

  // Give a dummy starting point
  typename F::argument_t x (f.inputSize ());
  x.setZero ();
  pb.startingPoint () = x;

  // Instantiate the factory using the dummy solver.
  SolverFactory<solver_t>
    factory (std::string ("dummy")
             + toString<typename F::parent_t>::value ()
             + "-laststate",
             pb);

  // Retrieve a solver instance from the factory.
  solver_t& solver = factory ();

  // Add optimization logger.
  OptimizationLogger<solver_t>
    logger (solver,
            std::string ("/tmp/roboptim-core-tests/optimization-logger-")
            + toString<F>::value ());

  // Get the Boost.Variant minimum.
  typename solver_t::result_t res = solver.minimum ();

  // Get the "real" minimum.
  solver.template getMinimum<SolverError> ();

  // Display problem and solver.
  (*output) << pb << std::endl
            << "---" << std::endl
            << solver;

  // Test whether the logging directory exists
  BOOST_CHECK (boost::filesystem::exists (logger.logPath ()));
  // Test whether journal.log exists
  BOOST_CHECK (boost::filesystem::exists (logger.logPath () / "journal.log"));
}

BOOST_AUTO_TEST_CASE (plugin)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("optimization-logger");

  testLogger<F1> (output);
  (*output) << "---" << std::endl;
  testLogger<F2> (output);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
