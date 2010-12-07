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

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;


// Define f(x) = x * x
struct Square : public Function
{
  explicit Square ()
    : Function (1, 1, "x * x")
  {
  }

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result[0] = argument[0] * argument[0];
  }
};

// Define a function that displays a circle.
struct Circle : public Function
{
  explicit Circle (double r = 1.)
    : Function (1, 2, "{sin (x) * r; cos (x) * r}"),
      r_ (r)
  {
  }

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result[0] = sin (argument[0]) * r_;
    result[1] = cos (argument[0]) * r_;
  }

  double r_;
};


int run_test ()
{
  using namespace roboptim::visualization::gnuplot;
  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();

  Square square;
  discreteInterval_t intervalS (-10., 10., 0.01);

  Circle circle;
  discreteInterval_t intervalC (0., 2 * M_PI, 0.01);

  std::cout
    << (gnuplot
	<< comment ("Hello, world (complex)!")
	<< set ("multiplot")
	<< plot (square, intervalS)
	<< plot_xy (circle, intervalC)
	<< unset ("multiplot")
	);
  return 0;
}

GENERATE_TEST ()
