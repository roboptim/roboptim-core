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

#include <boost/make_shared.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/util.hh>
#include <roboptim/core/portability.hh>
#include <roboptim/core/problem.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/numeric-linear-function.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define a simple function.
template <typename T>
struct F : public GenericFunction<T>
{
  ROBOPTIM_FUNCTION_FWD_TYPEDEFS_ (GenericFunction<T>);

  F () : GenericFunction<T> (2, 2, "a * b, a + b")
  {}

  void impl_compute (result_ref res, const_argument_ref x) const
  {
    res (0) = x[0] * x[1];
    res (1) = x[0] + x[1];
  }

  // No gradient, hessian.
};


BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (problem, T, functionTypes_t)
{
  output = retrievePattern ("problem");

  typedef Problem<T> problem_t;

  typedef typename problem_t::function_t      function_t;
  typedef typename function_t::argument_t     argument_t;
  typedef typename problem_t::startingPoint_t startingPoint_t;
  typedef typename problem_t::intervals_t     intervals_t;
  typedef typename problem_t::scaling_t       scaling_t;

  typedef GenericConstantFunction<T>          constantFunction_t;
  typedef GenericNumericLinearFunction<T>     numericLinearFunction_t;

  typename constantFunction_t::vector_t v (2);
  v.setZero ();

  // For shared_ptr constructor
  boost::shared_ptr<constantFunction_t>
    f = boost::make_shared<constantFunction_t> (v);

  problem_t pb_fail (f);

  argument_t bad_x (3);
  pb_fail.startingPoint () = bad_x;

  {
    BOOST_CHECK_THROW (startingPoint_t test_sp = pb_fail.startingPoint ();
                       (*output) << (*test_sp) << std::endl,
                       std::runtime_error);
  }

  {
    BOOST_CHECK_THROW (const startingPoint_t& test_sp = pb_fail.startingPoint ();
                       (*output) << (*test_sp) << std::endl,
                       std::runtime_error);
  }

  problem_t pb (f);
  argument_t x (2);
  x.setZero ();
  pb.startingPoint () = x;
  pb.argumentBounds ()[0] = function_t::makeInterval (-5., 5.);
  pb.argumentBounds ()[1] = function_t::makeLowerInterval (-10.);

  typename constantFunction_t::names_t names (2);
  names[0] = "x₀";
  names[1] = "x₁";
  pb.argumentNames () = names;

  typename numericLinearFunction_t::matrix_t a (2,2);
  typename numericLinearFunction_t::vector_t b (2);
  a.setZero ();
  a.coeffRef (0,0) = 100.;
  a.coeffRef (1,1) = 0.01;
  b << 200., -30.;

  boost::shared_ptr<numericLinearFunction_t>
    g0 = boost::make_shared<numericLinearFunction_t> (a, b);

  a.setZero ();
  a.coeffRef (0,0) = 10.;
  a.coeffRef (1,1) = -0.4;
  b << 2., 3.;
  boost::shared_ptr<numericLinearFunction_t>
    g1 = boost::make_shared<numericLinearFunction_t> (a, b);

  boost::shared_ptr<F<T> >
    g2 = boost::make_shared<F<T> > ();

  intervals_t intervals (2);
  scaling_t scaling (2, 1);

  scaling[0] = 1.;
  scaling[1] = 100.;
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInfiniteInterval ();
  pb.addConstraint (g0, intervals, scaling);

  scaling[0] = 10.;
  scaling[1] = 10.;
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeLowerInterval (0.);
  pb.addConstraint (g1, intervals, scaling);

  // Also add a non-differentiable constraint
  pb.argumentScaling ()[0] = 0.01;
  pb.argumentScaling ()[1] = 0.5;
  scaling[0] = 0.1;
  scaling[1] = 10.;
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInterval (-12., 42.);
  pb.addConstraint (g2, intervals, scaling);

  // Check jacobian
  x << 100., 2.;
  (*output) << toDense (pb.jacobian (x)) << std::endl;
  (*output) << toDense (pb.scaledJacobian (x)) << std::endl;

  // Check constraints output size
  BOOST_CHECK_EQUAL (pb.constraintsOutputSize (),
                     g0->outputSize () + g1->outputSize () + g2->outputSize ());
  BOOST_CHECK_EQUAL (pb.differentiableConstraintsOutputSize (),
                     g0->outputSize () + g1->outputSize ());

  // Check null ptr
  BOOST_CHECK_THROW (boost::shared_ptr<constantFunction_t> null_ptr;
                     pb.addConstraint (null_ptr, intervals, scaling),
                     std::runtime_error);

  // Check invalid input size
  BOOST_CHECK_THROW (typename constantFunction_t::vector_t v_size (4);
                     boost::shared_ptr<constantFunction_t>
                     cstr_size = boost::make_shared<constantFunction_t> (v_size);
                     pb.addConstraint (cstr_size, intervals, scaling),
                     std::runtime_error);

  // Check invalid interval size
  BOOST_CHECK_THROW (intervals_t intervals_size (6);
                     pb.addConstraint (g0, intervals_size, scaling),
                     std::runtime_error);

  // Check invalid scaling vector size
  BOOST_CHECK_THROW (scaling_t scaling_size (6);
                     pb.addConstraint (g0, intervals, scaling_size),
                     std::runtime_error);

  (*output) << pb << std::endl;

  // Backward compatibility
  // Disable deprecated warning
  ROBOPTIM_ALLOW_DEPRECATED_ON;

  BOOST_CHECK (pb.argumentScales () == pb.argumentScaling ());
  BOOST_CHECK (pb.scalesVector () == pb.scalingVector ());

  // For legacy const ref constructor
  constantFunction_t fRef (v);
  problem_t pbRef (fRef);
  (*output) << pbRef << std::endl;

  // Re-enable deprecated warning
  ROBOPTIM_ALLOW_DEPRECATED_OFF;

  // Clear problem's constraints.
  BOOST_CHECK (!pb.constraints ().empty ());
  BOOST_CHECK (!pb.boundsVector ().empty ());
  BOOST_CHECK (!pb.scalingVector ().empty ());
  BOOST_CHECK (pb.constraintsOutputSize () > 0);
  pb.clearConstraints ();
  BOOST_CHECK (pb.constraints ().empty ());
  BOOST_CHECK (pb.boundsVector ().empty ());
  BOOST_CHECK (pb.scalingVector ().empty ());
  BOOST_CHECK_EQUAL (pb.constraintsOutputSize (), 0);
  x << 0., 0.;
  pb.startingPoint () = x;
  (*output) << pb << std::endl;
  (*output) << pb.constraintsViolationVector (x) << std::endl;
  BOOST_CHECK_EQUAL (pb.template constraintsViolation<1> (x), 0.);
  BOOST_CHECK_EQUAL (pb.template constraintsViolation<Eigen::Infinity> (x), 0.);
  x << 200., 30.;
  pb.startingPoint () = x;
  (*output) << pb << std::endl;
  (*output) << pb.constraintsViolationVector (x) << std::endl;
  BOOST_CHECK_EQUAL (pb.template constraintsViolation<1> (x), 195.);
  BOOST_CHECK_EQUAL (pb.template constraintsViolation<Eigen::Infinity> (x), 195.);

  // Test a problem with multiple types of constraints.
  typedef Problem<T> mixedProblem_t;
  mixedProblem_t mixedPb (f);
  x << 1., 2.;
  mixedPb.startingPoint () = x;
  mixedPb.argumentNames () = names;
  mixedPb.argumentBounds ()[0] = function_t::makeUpperInterval (5.);

  v[0] = 3.;
  v[1] = -1.;
  boost::shared_ptr<constantFunction_t>
    g3 = boost::make_shared<constantFunction_t> (v);

  // First constraint: ConstantFunction automatically converted to LinearFunction
  mixedPb.addConstraint (g3, intervals, scaling);
  // Second constraint: NumericLinearFunction converted to DifferentiableFunction
  mixedPb.addConstraint (boost::static_pointer_cast<GenericDifferentiableFunction<T> > (g1),
                         intervals, scaling);

  // First constraint: LinearFunction
  BOOST_CHECK (mixedPb.constraints()[0]->template asType<constantFunction_t>());
  // Second constraint: DifferentiableFunction
  BOOST_CHECK (mixedPb.constraints()[1]->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK_EQUAL (mixedPb.constraintsOutputSize (), 2 * g0->outputSize ());
  x << 0., 0.;
  mixedPb.startingPoint () = x;
  (*output) << mixedPb << std::endl;
  (*output) << mixedPb.constraintsViolationVector (x) << std::endl;
  BOOST_CHECK_EQUAL (mixedPb.template constraintsViolation<1> (x), 0.);
  BOOST_CHECK_EQUAL (mixedPb.template constraintsViolation<Eigen::Infinity> (x), 0.);
  x << 200., 50.;
  mixedPb.startingPoint () = x;
  (*output) << mixedPb << std::endl;
  (*output) << mixedPb.constraintsViolationVector (x) << std::endl;
  BOOST_CHECK_EQUAL (mixedPb.template constraintsViolation<1> (x), 2160.);
  BOOST_CHECK_EQUAL (mixedPb.template constraintsViolation<Eigen::Infinity> (x), 1960.);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
