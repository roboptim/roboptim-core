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

#include <iostream>

#include <boost/numeric/ublas/io.hpp>

#include "common.hh"
#include <roboptim/core/derivable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

struct Null : public DerivableFunction
{
  Null () : DerivableFunction (1, 1, "null function")
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    res.clear ();
  }

  void impl_gradient (gradient_t& grad, const argument_t& argument,
		      size_type) const throw ()
  {
    grad.clear ();
  }
};

struct NoTitle : public DerivableFunction
{
  NoTitle () : DerivableFunction (1, 1)
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    res.clear ();
  }

  void impl_gradient (gradient_t& grad, const argument_t& argument,
		      size_type) const throw ()
  {
    grad.clear ();
  }
};

int run_test ()
{
  Null null;
  NoTitle notitle;

  std::cout << null << std::endl
	    << notitle << std::endl;

  Null::vector_t x (1);
  Null::gradient_t grad (null.gradientSize ());
  x[0] = 42.;

  std::cout << null.gradient (x) << std::endl
	    << notitle.gradient (x) << std::endl;

  null.gradient (grad, x);
  std::cout << grad << std::endl;
  notitle.gradient (grad, x);
  std::cout << grad << std::endl;

  std::cout << null.gradientSize () << std::endl
	    << notitle.gradientSize () << std::endl;

  std::cout << null.jacobianSize () << std::endl
	    << notitle.jacobianSize () << std::endl;

  std::cout << null.isValidGradient (null.gradient (x)) << std::endl
	    << notitle.isValidGradient (notitle.gradient (x)) << std::endl;

  std::cout << null.isValidJacobian (null.jacobian (x)) << std::endl
	    << notitle.isValidJacobian (notitle.jacobian (x)) << std::endl;

  return 0;
}

GENERATE_TEST ()
