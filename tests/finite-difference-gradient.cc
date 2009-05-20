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

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[0];
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int) const throw ()
  {
    vector_t res (n);
    res (0) = 2 * x[0];
    return res;
  }
};


struct FBad : public DerivableFunction
{
  FBad () : DerivableFunction (1, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[0];
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int) const throw ()
  {
    vector_t res (n);
    res (0) = 5 * x[0] + 42;
    return res;
  }
};


struct Polynomial : public DerivableFunction
{
  Polynomial () : DerivableFunction (1, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = -24 * x[0] * x[0] + 33 * x[0] + 5;
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int) const throw ()
  {
    vector_t res (n);
    res (0) = -42 * x[0] + 33;
    return res;
  }
};


struct SquareXY : public DerivableFunction
{
  SquareXY () : DerivableFunction (1, 2)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = sin (x[0]);
    res (1) = cos (x[0]);
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int i) const throw ()
  {
    vector_t res (n);
    switch (i)
      {
      case 0:
	res (0) = cos (x[0]);
	break;

      case 1:
	res (0) = -sin (x[0]);
	break;

      default:
	assert (0);
      }

    return res;
  }
};

struct Times : public DerivableFunction
{
  Times () : DerivableFunction (2, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[1];
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int i) const throw ()
  {
    vector_t res (n);
    res (0) = x[1];
    res (1) = x[0];
    return res;
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
