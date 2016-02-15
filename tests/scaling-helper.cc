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

#include <roboptim/core/io.hh>
#include <roboptim/core/util.hh>
#include <roboptim/core/problem.hh>
#include <roboptim/core/scaling-helper.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>

using namespace roboptim;

typedef boost::mpl::list<EigenMatrixDense> functionTypes_t;

// Output stream
boost::shared_ptr<boost::test_tools::output_test_stream> output;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (scaling_helper, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("scaling-helper");

  typedef Problem<T> problem_t;
  typedef typename problem_t::function_t      function_t;
  typedef typename function_t::argument_t     argument_t;
  typedef typename problem_t::intervals_t     intervals_t;
  typedef typename problem_t::interval_t      interval_t;
  typedef typename problem_t::scaling_t       scaling_t;

  typedef GenericConstantFunction<T>          constantFunction_t;
  typedef GenericNumericLinearFunction<T>     numericLinearFunction_t;
  typedef GenericNumericQuadraticFunction<T>  numericQuadraticFunction_t;

  // Initialize problem
  const size_t n = 2;
  typename constantFunction_t::vector_t v (n);
  v << 1., 2.;
  boost::shared_ptr<constantFunction_t>
    f = boost::make_shared<constantFunction_t> (v);
  (*output) << *f << std::endl;

  problem_t pb (f);
  (*output) << pb << std::endl;

  std::vector<argument_t> x (2, argument_t::Zero (n));
  x[0] << 2., 1.;
  x[1] << 100., -200.;

  (*output) << x[0] << std::endl;
  (*output) << x[1] << std::endl;

  // Check without constraints
  {
    ScalingHelper<T> sh (pb, function_t::makeInterval (1e-1, 1e2));
    (*output) << sh << std::endl;
  }

  // Add some dummy constraints
  typename numericLinearFunction_t::matrix_t a (2,2);
  a.setZero ();
  a.coeffRef (0,0) = 300.;
  a.coeffRef (1,1) = 10.;
  typename numericLinearFunction_t::vector_t b (2);
  b << 200., 300.;
  boost::shared_ptr<numericLinearFunction_t>
    g0 = boost::make_shared<numericLinearFunction_t> (a, b);
  intervals_t intervals (2);
  scaling_t scaling (2, 1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeLowerInterval (0.);
  pb.addConstraint (g0, intervals, scaling);

  boost::shared_ptr<numericQuadraticFunction_t>
    g1 = boost::make_shared<numericQuadraticFunction_t> (a, b);
  intervals.resize (1);
  scaling.resize (1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeLowerInterval (0.);
  pb.addConstraint (g1, intervals, scaling);

  a *= 1e-6;
  b *= 1e-6;
  boost::shared_ptr<numericQuadraticFunction_t>
    g2 = boost::make_shared<numericQuadraticFunction_t> (a, b);
  intervals.resize (1);
  scaling.resize (1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeLowerInterval (0.);
  pb.addConstraint (g2, intervals, scaling);

  (*output) << pb << std::endl;

  // Check with constraints
  {
    ScalingHelper<T> sh (pb, function_t::makeInterval (1e-1, 1e2));
    (*output) << Eigen::MatrixXd (pb.jacobian (x[0])) << std::endl;
    (*output) << Eigen::MatrixXd (pb.jacobian (x[1])) << std::endl;
    (*output) << sh << std::endl;
    sh.computeScaling (x);
    (*output) << sh << std::endl;
  }

  {
    interval_t bounds;
    bounds.first = -1.;
    bounds.second = 100.;
    BOOST_CHECK_THROW (ScalingHelper<T> sh1 (pb, bounds),
          std::runtime_error);
    bounds.first = 200.;
    BOOST_CHECK_THROW (ScalingHelper<T> sh2 (pb, bounds),
          std::runtime_error);
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
