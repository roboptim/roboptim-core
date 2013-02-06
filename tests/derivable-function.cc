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
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

struct Null : public DifferentiableFunction
{
  Null () : DifferentiableFunction (1, 1, "null function")
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
    res[0] = 41.;
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
    grad (0, 0) = 1.;
  }
};

struct NoTitle : public DifferentiableFunction
{
  NoTitle () : DifferentiableFunction (1, 1)
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
    res[0] = 42.;
  }

  void impl_gradient (gradient_t grad, argument_t,
		      size_type) const throw ()
  {
    grad.block (0, 0, grad.rows (), grad.cols ()).setZero ();
    grad (0, 0) = 2.;
  }
};

BOOST_AUTO_TEST_CASE (derivable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("derivable-function");

  Null null;
  NoTitle notitle;

  Null::vector_t x (1);
  Null::vector_t grad (null.gradientSize ());
  grad.setZero ();
  x[0] = 42.;

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

  (*output) << "xxx " << null.gradient (x) << std::endl
	    << "yyy " << notitle.gradient (x) << std::endl;

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
