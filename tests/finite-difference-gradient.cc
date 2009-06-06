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
#include <boost/variant/get.hpp>

#include "common.hh"
#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/indent.hh>

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;
using namespace roboptim::visualization::gnuplot;

struct FGood : public DerivableFunction
{
  FGood () : DerivableFunction (1, 1)
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


struct FBad : public DerivableFunction
{
  FBad () : DerivableFunction (1, 1)
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


struct SquareXY : public DerivableFunction
{
  SquareXY () : DerivableFunction (1, 2)
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
		      size_type idFunction) const throw ()
  {
    gradient (0) = argument[1];
    gradient (1) = argument[0];
  }
};

void displayGradient (const DerivableFunction&,
		      const Function::vector_t&,
		      int i = 0);

void
displayGradient (const DerivableFunction& function,
		 const Function::vector_t& x,
		 int i)
{
  FiniteDifferenceGradient fdfunction (function);
  DerivableFunction::gradient_t grad = function.gradient (x, i);
  DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

  std::cout << "#" << grad << std::endl
	    << "#" << fdgrad << std::endl;
}

int run_test ()
{
  FGood fg;
  FBad fb;
  SquareXY sq;
  Times times;

  Function::vector_t x (1);

  for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
    {
      std::cout << "#Check gradient at x = " << x[0] << std::endl;

      std::cout << "# Good" << std::endl;
      displayGradient (fg, x);

      std::cout << "# Bad" << std::endl;
      displayGradient (fb, x);

      std::cout << "# Square" << std::endl;
      displayGradient (sq, x);
      displayGradient (sq, x, 1);

      assert (checkGradient (fg, 0, x));
      assert (! checkGradient (fb, 0, x));

      assert (checkGradient (sq, 0, x));
      assert (checkGradient (sq, 1, x));
    }

  x.resize (2);
  for (x[1] = -10.; x[1] < 10.; x[1] += 1.)
    for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
      {
	std::cout << "# Times at x = " << x << std::endl;
	displayGradient (times, x);
	assert (checkGradient (times, 0, x));
      }

  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();

  FiniteDifferenceGradient fg_fd (fg, 10.);

  Polynomial p;
  FiniteDifferenceGradient p_fd (p, 10.);

  Function::discreteInterval_t interval (-100., 100., 1.);

  std::cout
    << (gnuplot
	<< set ("multiplot layout 2,2")
	<< plot (fg, interval)
	<< plot (fg_fd, interval)
	<< plot (p, interval)
	<< plot (p_fd, interval)
	<< unset ("multiplot")
	);

  return 0;
}

GENERATE_TEST ()
