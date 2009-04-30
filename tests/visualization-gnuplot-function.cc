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

#include "common.hh"
#include <roboptim-core/visualization/gnuplot.hh>
#include <roboptim-core/visualization/gnuplot-commands.hh>
#include <roboptim-core/visualization/gnuplot-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;


struct Square : public Function
{
  explicit Square ()
    : Function (1, 1)
  {
  }
  vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (n);
    res[0] = x[0] * x[0];
    return res;
  }
};

struct Circle : public Function
{
  explicit Circle (double r = 1.)
    : Function (2, 2),
      r_ (r)
  {
  }
  vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (n);
    res[0] = cos (x[0]);
    res[1] = sin (x[1]);
    return res;
  }

  double r_;
};


int run_test ()
{
  using namespace roboptim::visualization::gnuplot;
  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();

  Square square;
  discreteInterval_t interval (-10., 10., 0.01);

  std::cout
    << (gnuplot
	<< comment ("Hello, world (complex)!")
	<< set ("term", "x11 enhanced persist")
	<< plot (square, interval)
	);
  return 0;
}

GENERATE_TEST ()
