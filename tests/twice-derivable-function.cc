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

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/twice-differentiable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

struct Null : public TwiceDifferentiableFunction
{
  Null () : TwiceDifferentiableFunction (1, 1, "null function")
  {}

  void impl_compute (result_ref res, const_argument_ref) const throw ()
  {
    res.setZero ();
  }

  void impl_gradient (gradient_ref grad, const_argument_ref,
		      size_type) const
  {
    grad.setZero ();
  }

  void impl_hessian (hessian_ref h, const_argument_ref,
		     size_type) const
  {
    h.setZero ();
  }
};

struct NoTitle : public TwiceDifferentiableFunction
{
  NoTitle () : TwiceDifferentiableFunction (1, 1)
  {}

  void impl_compute (result_ref res, const_argument_ref) const throw ()
  {
    res.setZero ();
  }

  void impl_gradient (gradient_ref grad, const_argument_ref,
		      size_type) const
  {
    grad.setZero ();
  }

  void impl_hessian (hessian_ref h, const_argument_ref, size_type)
    const
  {
    h.setZero ();
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (twice_derivable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("twice-derivable-function");

  Null null;
  NoTitle notitle;

  Null::vector_t x (1);
  x[0] = 42.;
  Null::hessian_t h (null.hessianSize ().first,
		     null.hessianSize ().second);
  Null::gradient_t grad (null.gradientSize ());

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


  (*output) << null.hessian (x) << std::endl
	    << notitle.hessian (x) << std::endl;

  null.hessian (h, x);
  (*output) << h << std::endl;
  notitle.hessian (h, x);
  (*output) << h << std::endl;


  (*output) << null.hessianSize () << std::endl
	    << notitle.hessianSize () << std::endl;

  (*output) << null.isValidHessian (null.hessian (x)) << std::endl
	    << notitle.isValidHessian (notitle.hessian (x)) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
