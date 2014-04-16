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

#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

template <typename T>
struct Null : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  Null () : GenericDifferentiableFunction<T> (1, 1, "null function")
  {}

  void impl_compute (result_ref res, const_argument_ref) const
  {
    res.setZero ();
  }

  void impl_gradient (gradient_ref grad, const_argument_ref,
		      size_type) const
  {
    grad.setZero ();
  }
};

template <typename T>
struct NoTitle : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  NoTitle () : GenericDifferentiableFunction<T> (1, 1)
  {}

  void impl_compute (result_ref res, const_argument_ref) const
  {
    res.setZero ();
  }

  void impl_gradient (gradient_ref grad, const_argument_ref,
		      size_type) const
  {
    grad.setZero ();
  }
};

template <typename T>
struct getPattern
{
  const char* operator () ();
};

template <>
const char* getPattern< ::roboptim::EigenMatrixSparse>::operator () ()
{
  return "derivable-function-sparse";
}

template <typename T>
const char* getPattern<T>::operator () ()
{
  return "derivable-function";
}

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (derivable_function, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern (getPattern<T> () ());

  Null<T> null;
  NoTitle<T> notitle;

  typename Null<T>::argument_t x (1);
  typename Null<T>::gradient_t grad (null.gradientSize ());
  x[0] = 42.;

  (*output) << null << std::endl
	    << notitle << std::endl;

  (*output) << null.inputSize () << std::endl
	    << notitle.inputSize () << std::endl;

  (*output) << null.outputSize () << std::endl
	    << notitle.outputSize () << std::endl;

  (*output) << null.getName () << std::endl
	    << notitle.getName () << std::endl;

  (*output) << null.isValidResult (null (x)) << std::endl
	    << notitle.isValidResult (notitle (x)) << std::endl;

  (*output) << null (x) << std::endl
	    << notitle (x) << std::endl;

  (*output) << null.gradient (x) << std::endl
	    << notitle.gradient (x) << std::endl;

  null.gradient (grad, x);
  (*output) << grad << std::endl;
  notitle.gradient (grad, x);
  (*output) << grad << std::endl;

  (*output) << null.gradientSize () << std::endl
	    << notitle.gradientSize () << std::endl;

  (*output) << null.jacobianSize () << std::endl
	    << notitle.jacobianSize () << std::endl;

  (*output) << null.isValidGradient (null.gradient (x)) << std::endl
	    << notitle.isValidGradient (notitle.gradient (x)) << std::endl;

  (*output) << null.isValidJacobian (null.jacobian (x)) << std::endl
	    << notitle.isValidJacobian (notitle.jacobian (x)) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}


template <typename T>
struct F : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  F () : GenericDifferentiableFunction<T> (4, 2, "derivable function")
  {}

  void impl_compute (result_ref res, const_argument_ref x) const
  {
    res[0] = x[0] * x[1];
    res[1] = x[2] * x[3];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref x,
		      size_type functionId) const;
};

template <>
void
F<roboptim::EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type functionId) const
{
  grad.resize (4);
  if (functionId == 0)
    {
      grad.insert (0) = x[1];
      grad.insert (1) = x[0];
      grad.insert (2) = 0.;
      grad.insert (3) = 0.;
    }
  else if (functionId == 1)
    {
      grad.insert (0) = 0.;
      grad.insert (1) = 0.;
      grad.insert (2) = x[3];
      grad.insert (3) = x[2];
    }
  else
    assert (0);
}

template <typename T>
void
F<T>::impl_gradient (gradient_ref grad, const_argument_ref x,
		     size_type functionId) const
{
  if (functionId == 0)
    {
      grad[0] = x[1];
      grad[1] = x[0];
      grad[2] = 0.;
      grad[3] = 0.;
    }
  else if (functionId == 1)
    {
      grad[0] = 0.;
      grad[1] = 0.;
      grad[2] = x[3];
      grad[3] = x[2];
    }
  else
    assert (0);
}


BOOST_AUTO_TEST_CASE_TEMPLATE (jacobian_check, T, functionTypes_t)
{
  F<T> f;
  typename F<T>::argument_t x (4);
  x[0] = 10.;
  x[1] = -1.;
  x[2] = 7.;
  x[3] = 2.;

  BOOST_REQUIRE_EQUAL (f.jacobian (x).row (0).size (),
		       f.gradient (x, 0).size ());
  BOOST_REQUIRE_EQUAL (f.jacobian (x).row (1).size (),
		       f.gradient (x, 1).size ());

  BOOST_CHECK (f.jacobian (x).row (0).isApprox (f.gradient (x, 0).adjoint ()));
  BOOST_CHECK (f.jacobian (x).row (1).isApprox (f.gradient (x, 1).adjoint ()));

  typename F<T>::jacobian_t jac = f.jacobian (x);
  typename F<T>::gradient_t grad0 = f.gradient (x, 0);
  typename F<T>::gradient_t grad1 = f.gradient (x, 1);

  // Call f with a map to a C-style array.
  typename F<T>::value_type ar[4] = {10., -1., 7., 2.};
  Eigen::Map<typename F<T>::argument_t> map_x (ar, 4);
  BOOST_CHECK (f.jacobian (map_x).row (0).isApprox (f.gradient (map_x, 0).adjoint ()));
  BOOST_CHECK (f.jacobian (map_x).row (1).isApprox (f.gradient (map_x, 1).adjoint ()));
  BOOST_CHECK (f.jacobian (map_x).isApprox (jac));
  BOOST_CHECK (f.gradient (map_x, 0).isApprox (grad0));
  BOOST_CHECK (f.gradient (map_x, 1).isApprox (grad1));
  BOOST_CHECK_SMALL (f (x)[0] - f (map_x)[0], 1e-6);

  // Call f with a block of a vector.
  typename F<T>::argument_t full_x (8);
  full_x.setZero ();
  full_x.segment (2,4) = x;
  BOOST_CHECK (f.jacobian (full_x.segment (2,4)).row (0).isApprox
               (f.gradient (full_x.segment (2,4), 0).adjoint ()));
  BOOST_CHECK (f.jacobian (full_x.segment (2,4)).row (1).isApprox
               (f.gradient (full_x.segment (2,4), 1).adjoint ()));
  BOOST_CHECK (f.jacobian (full_x.segment (2,4)).isApprox (jac));
  BOOST_CHECK (f.gradient (full_x.segment (2,4), 0).isApprox (grad0));
  BOOST_CHECK (f.gradient (full_x.segment (2,4), 1).isApprox (grad1));
  BOOST_CHECK_SMALL (f (x)[0] - f (full_x.segment (2,4))[0], 1e-6);
}

BOOST_AUTO_TEST_SUITE_END ()
