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

#include "shared-tests/common.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/twice-differentiable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

struct Null : public TwiceDifferentiableFunction
{
  Null () : TwiceDifferentiableFunction (1, 1, "null function")
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
  }

  void impl_hessian (hessian_t h, argument_t,
		     size_type) const throw ()
  {
    h.block (0, 0, h.rows (), h.cols ()).setZero ();
  }
};

struct NoTitle : public TwiceDifferentiableFunction
{
  NoTitle () : TwiceDifferentiableFunction (1, 1)
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
  }

  void impl_hessian (hessian_t h, argument_t, size_type)
    const throw ()
  {
    h.block (0, 0, h.rows (), h.cols ()).setZero ();
  }
};

BOOST_AUTO_TEST_CASE (twice_derivable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("twice-derivable-function");

  Null null;
  NoTitle notitle;

  Null::vector_t x (1);
  x[0] = 42.;
  Null::matrix_t h (null.hessianSize ().first,
		     null.hessianSize ().second);
  Null::vector_t grad (null.gradientSize ());

  (*output) << null << std::endl
	    << notitle << std::endl;

  (*output) << null.inputSize () << std::endl
	    << notitle.inputSize () << std::endl;

  (*output) << null.outputSize () << std::endl
	    << notitle.outputSize () << std::endl;

  (*output) << null.getName () << std::endl
	    << notitle.getName () << std::endl;

  Null::vector_t r = null (x);
  Null::vector_t r2 = notitle (x);
  (*output) << null.isValidResult (r) << std::endl
	    << notitle.isValidResult (r2) << std::endl;

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

  Null::matrix_t hess = null.hessian (x);
  Null::matrix_t hess2 = notitle.hessian (x);
  (*output) << null.isValidHessian (hess) << std::endl
	    << notitle.isValidHessian (hess2) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
