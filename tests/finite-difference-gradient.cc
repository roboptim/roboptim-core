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
#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/indent.hh>

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;
using namespace roboptim::visualization::gnuplot;

// Define a function with a correct gradient.
struct FGood : public DerivableFunction
{
  FGood () : DerivableFunction (1, 1, "x * x")
  {}

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result (0) = argument[0] * argument[0];
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t& argument, size_type) const throw ()
  {
    gradient (0) = 2 * argument[0];
  }
};

// Define a function with a bad gradient.
struct FBad : public DerivableFunction
{
  FBad () : DerivableFunction (1, 1, "x * x")
  {}

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result (0) = argument[0] * argument[0];
  }

  void impl_gradient (result_t& result,
		      const vector_t& argument, size_type) const throw ()
  {
    result (0) = 5 * argument[0] + 42;
  }
};

// Define a polynomial function.
struct Polynomial : public DerivableFunction
{
  Polynomial () : DerivableFunction (1, 1)
  {}

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result (0) = -24 * argument[0] * argument[0] + 33 * argument[0] + 5;
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t& argument, size_type) const throw ()
  {
    gradient (0) = -42 * argument[0] + 33;
  }
};

// Define a function that draws a circle.
struct CircleXY : public DerivableFunction
{
  CircleXY () : DerivableFunction (1, 2)
  {}

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result (0) = sin (argument[0]);
    result (1) = cos (argument[0]);
  }

  void impl_gradient (result_t& result,
		      const argument_t& argument,
		      size_type idFunction) const throw ()
  {
    switch (idFunction)
      {
      case 0:
	result (0) = cos (argument[0]);
	break;

      case 1:
	result (0) = -sin (argument[0]);
	break;

      default:
	assert (0);
      }
  }
};

// Define ``f(x,y) = x * y'' function.
struct Times : public DerivableFunction
{
  Times () : DerivableFunction (2, 1)
  {}

  void impl_compute (result_t& result,
		     const vector_t& argument) const throw ()
  {
    result (0) = argument[0] * argument[1];
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t& argument,
		      size_type) const throw ()
  {
    gradient (0) = argument[1];
    gradient (1) = argument[0];
  }
};

void displayGradient
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const DerivableFunction&,
 const Function::vector_t&,
 Function::size_type i = 0);

void
displayGradient
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const DerivableFunction& function,
 const Function::vector_t& x,
 Function::size_type i)
{
  FiniteDifferenceGradient<> fdfunction (function);
  DerivableFunction::gradient_t grad = function.gradient (x, i);
  DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

  (*output) << "#" << grad << std::endl
	    << "#" << fdgrad << std::endl;
}

BOOST_AUTO_TEST_CASE (finite_difference_gradient)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("finite-difference-gradient");

  FGood fg;
  FBad fb;
  CircleXY sq;
  Times times;

  Function::vector_t x (1);

  for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
    {
      (*output) << "#Check gradient at x = " << x[0] << std::endl;

      (*output) << "# Good" << std::endl;
      displayGradient (output, fg, x);

      (*output) << "# Bad" << std::endl;
      displayGradient (output, fb, x);

      (*output) << "# Circle" << std::endl;
      displayGradient (output, sq, x);
      displayGradient (output, sq, x, 1);

      BOOST_CHECK (checkGradient (fg, 0, x));
      BOOST_CHECK (! checkGradient (fb, 0, x));

      BOOST_CHECK (checkGradient (sq, 0, x));
      BOOST_CHECK (checkGradient (sq, 1, x));
    }

  x.resize (2);
  for (x[1] = -10.; x[1] < 10.; x[1] += 1.)
    for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
      {
	(*output) << "# Times at x = " << x << std::endl;
	displayGradient (output, times, x);
	BOOST_CHECK (checkGradient (times, 0, x));
      }

  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();

  FiniteDifferenceGradient<> fg_fd (fg, 10.);

  Polynomial p;
  FiniteDifferenceGradient<finiteDifferenceGradientPolicies::Simple>
    p_fd (p, 10.);

  Function::discreteInterval_t interval (-100., 100., 1.);

  (*output)
    << (gnuplot
	<< set ("multiplot layout 2,2")
	<< plot (fg, interval)
	<< plot (fg_fd, interval)
	<< plot (p, interval)
	<< plot (p_fd, interval)
	<< unset ("multiplot")
	);
}
