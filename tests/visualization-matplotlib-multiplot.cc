// Copyright (C) 2015 by Félix Darricau, AIST, CNRS, EPITA
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

#include <roboptim/core/visualization/matplotlib.hh>
#include <roboptim/core/visualization/matplotlib-commands.hh>
#include <roboptim/core/visualization/matplotlib-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;


// Define f(x) = x * x
struct Square : public Function
{
  explicit Square ()
    : Function (1, 1, "x * x")
  {
  }

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
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

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result[0] = sin (argument[0]) * r_;
    result[1] = cos (argument[0]) * r_;
  }

  double r_;
};

// Define a function that displays a cubic and a quartic monomial.
struct Poly : public Function
{
  explicit Poly ()
    : Function (1, 2, "{x^3; x^4}")
  {
  }

  void impl_compute (result_ref result,
                     const_argument_ref argument) const
  {
    result[0] = argument[0] * argument[0] * argument[0];
    result[1] = result[0] * argument[0];
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (visualization_matplotlib_multiplot)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("visualization-matplotlib-multiplot");

  using namespace roboptim::visualization::matplotlib;
  Matplotlib matplotlib = Matplotlib::make_matplotlib (std::make_pair(2, 2));

  Square square;
  discreteInterval_t intervalS (-10., 10., 0.01);

  Circle circle;
  discreteInterval_t intervalC (0., 2 * M_PI, 0.01);

  Poly poly;
  discreteInterval_t intervalP (-1., 1., 0.01);

  (*output)
    << (matplotlib
	<< plot (square, intervalS)
	<< title ("Such a beautiful square") // Beware, you should always call the
	// title of your plot after the actual
	// plot. That's how matplotlib works.
	<< plot (circle, intervalC)
	<< title ("Such a beautiful circle")
	<< plot (poly, intervalP)
	<< title ("Such beautiful monomials")
	);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
