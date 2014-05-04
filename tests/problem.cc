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

#include <roboptim/core/io.hh>
#include <roboptim/core/problem.hh>
#include <roboptim/core/function/constant.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (problem)
{
  typedef Problem<DifferentiableFunction,
		  boost::mpl::vector<DifferentiableFunction> >
    problem_t;
  typedef Function::argument_t argument_t;
  typedef problem_t::startingPoint_t startingPoint_t;

  ConstantFunction::vector_t v (2);
  v.setZero ();

  ConstantFunction f (v);

  problem_t pb_fail (f);

  argument_t bad_x (3);
  pb_fail.startingPoint () = bad_x;

  {
    BOOST_CHECK_THROW (startingPoint_t test_sp = pb_fail.startingPoint ();
                       std::cout << (*test_sp) << std::endl,
                       std::runtime_error);
  }

  {
    BOOST_CHECK_THROW (const startingPoint_t& test_sp = pb_fail.startingPoint ();
                       std::cout << (*test_sp) << std::endl,
                       std::runtime_error);
  }

  problem_t pb (f);
  argument_t x (2);
  x.setZero ();
  pb.startingPoint () = x;

  boost::shared_ptr<ConstantFunction>
    cstr = boost::make_shared<ConstantFunction>  (v);
  problem_t::intervals_t intervals (2);
  problem_t::scales_t scales (2, 1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInfiniteInterval ();
  pb.addConstraint (cstr, intervals, scales);

  // Check null ptr
  BOOST_CHECK_THROW (boost::shared_ptr<ConstantFunction> null_ptr;
                     pb.addConstraint (null_ptr, intervals, scales),
                     std::runtime_error);

  // Check invalid input size
  BOOST_CHECK_THROW (ConstantFunction::vector_t v_size (4);
                     boost::shared_ptr<ConstantFunction>
                     cstr_size = boost::make_shared<ConstantFunction> (v_size);
                     pb.addConstraint (cstr_size, intervals, scales),
                     std::runtime_error);

  // Check invalid interval size
  BOOST_CHECK_THROW (problem_t::intervals_t intervals_size (6);
                     pb.addConstraint (cstr, intervals_size, scales),
                     std::runtime_error);

  // Check invalid scale vector size
  BOOST_CHECK_THROW (problem_t::scales_t scales_size (6);
                     pb.addConstraint (cstr, intervals, scales_size),
                     std::runtime_error);

  std::cout << pb << std::endl;
}

BOOST_AUTO_TEST_SUITE_END ()
