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

#include <boost/mpl/vector.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/problem.hh>

using namespace roboptim;

// Check that a problem has really been copied.
#define CHECK_COPY(A, B)                                             \
  assert (&(A).function () == &(B).function ());                     \
  assert ((A).constraints ().size () == (B).constraints ().size ()); \
  assert ((A).argumentNames ().size () == (B).argumentNames ().size ());

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (problem_copy_constructor)
{
  typedef Problem<EigenMatrixDense> problemSrc_t;
  typedef Problem<EigenMatrixDense> problemDst_t;
  typedef Problem<EigenMatrixDense> ambiguousProblemDst_t;

  ConstantFunction::vector_t v (1);
  v.setZero ();

  ConstantFunction f (v);

  problemSrc_t pbSrc (f);

  ConstantFunction::names_t names (1);
  names[0] = "x";
  pbSrc.argumentNames () = names;

  boost::shared_ptr<ConstantFunction>
    cstr = boost::make_shared<ConstantFunction>  (v);
  problemSrc_t::intervals_t intervals (1);
  problemSrc_t::scaling_t scaling (1, 1);
  for (size_t i = 0; i < intervals.size (); ++i)
    intervals[i] = Function::makeInfiniteInterval ();

  // Add ConstantFunction constraint.
  pbSrc.addConstraint (cstr, intervals, scaling);

  // Add DifferentiableFunction constraint.
  pbSrc.addConstraint (boost::static_pointer_cast<DifferentiableFunction> (cstr),
                       intervals, scaling);

  // Check with same type.
  {
    problemSrc_t pbDst (pbSrc);
    CHECK_COPY(pbSrc, pbDst);
    BOOST_CHECK(pbDst.constraints ()[0].which () == 0);
    BOOST_CHECK(pbDst.constraints ()[1].which () == 1);
  }

  // Check with a more general type.
  {
    problemDst_t pbDst (pbSrc);
    CHECK_COPY(pbSrc, pbDst);
  }

  // Check with ambiguous constraints types.
  {
    ambiguousProblemDst_t pbDst (pbSrc);
    CHECK_COPY(pbSrc, pbDst);
    BOOST_CHECK(pbDst.constraints ()[0].which () == 0);
    BOOST_CHECK(pbDst.constraints ()[1].which () == 1);
  }

  // With invalid constraints types, compilation would fail.
  //{
  //  typedef Problem<Function, boost::mpl::vector<TwiceDifferentiableFunction> >
  //    wrongProblemDst_t;

  //  wrongProblemDst_t pbDst (pbSrc);
  //  CHECK_COPY(pbSrc, pbDst);
  //}
}

BOOST_AUTO_TEST_SUITE_END ()
