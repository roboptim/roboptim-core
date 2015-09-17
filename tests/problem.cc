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
#include <roboptim/core/problem.hh>
#include <roboptim/core/function/constant.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

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

  typename constantFunction_t::names_t names (2);
  names[0] = "x₀";
  names[1] = "x₁";
  pb.argumentNames () = names;

  boost::shared_ptr<constantFunction_t>
    cstr = boost::make_shared<constantFunction_t>  (v);
  intervals_t intervals (2);
  scaling_t scaling (2, 1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInfiniteInterval ();
  pb.addConstraint (cstr, intervals, scaling);

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
                     pb.addConstraint (cstr, intervals_size, scaling),
                     std::runtime_error);

  // Check invalid scaling vector size
  BOOST_CHECK_THROW (scaling_t scaling_size (6);
                     pb.addConstraint (cstr, intervals, scaling_size),
                     std::runtime_error);

  (*output) << pb << std::endl;

  // Backward compatibility
  // Disable deprecated warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  BOOST_CHECK (pb.argumentScales () == pb.argumentScaling ());
  BOOST_CHECK (pb.scalesVector () == pb.scalingVector ());

  // For legacy const ref constructor
  constantFunction_t fRef (v);
  problem_t pbRef (fRef);
  (*output) << pbRef << std::endl;
#pragma GCC diagnostic pop

  // Test a problem with multiple types of constraints.
  typedef Problem<T> mixedProblem_t;
  mixedProblem_t mixedPb (f);
  mixedPb.startingPoint () = x;
  mixedPb.argumentNames () = names;

  // First constraint: ConstantFunction automatically converted to LinearFunction
  mixedPb.addConstraint (cstr, intervals, scaling);
  // Second constraint: ConstantFunction converted to DifferentiableFunction
  mixedPb.addConstraint (boost::static_pointer_cast<GenericDifferentiableFunction<T> > (cstr),
                         intervals, scaling);

  // First constraint: LinearFunction
  BOOST_CHECK (mixedPb.constraints()[0]->template asType<constantFunction_t>());
  // Second constraint: DifferentiableFunction
  BOOST_CHECK (mixedPb.constraints()[1]->template asType<GenericDifferentiableFunction<T> >());

  (*output) << mixedPb << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
