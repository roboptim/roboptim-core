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
#include <roboptim/core/visualization/gnuplot-differentiable-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;


// Define f(x) = forty_two(x)
struct Square : public DifferentiableFunction
{
  explicit Square ()
    : DifferentiableFunction (7, 7, "The Answer")
  {
  }

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result[0] = argument[0] + argument[4] + argument[5];
    result[1] = argument[0] + argument[2] + argument[6];
    result[2] = argument[0] + argument[2] + argument[6];
    result[3] = argument[0] + argument[1] + argument[2] + argument[5];
    result[4] = argument[2] + argument[4];
    result[5] = argument[2] + argument[4];
    result[6] = argument[2] + argument[4] + argument[5] + argument[6];
  }

  void impl_gradient (gradient_t&, const argument_t&,  size_type)
    const throw ()
  {
  }

  void impl_jacobian (jacobian_t& jac, const argument_t&)
    const throw ()
  {
    jac.setZero();
    jac(0,0) = 1.0;
    jac(0,4) = 1.0;
    jac(0,5) = 1.0;
    jac(1,0) = 1.0;
    jac(1,2) = 1.0;
    jac(1,6) = 1.0;
    jac(2,0) = 1.0;
    jac(2,2) = 1.0;
    jac(2,6) = 1.0;
    jac(3,0) = 1.0;
    jac(3,1) = 1.0;
    jac(3,2) = 1.0;
    jac(3,5) = 1.0;
    jac(4,2) = 1.0;
    jac(4,4) = 1.0;
    jac(5,2) = 1.0;
    jac(5,4) = 1.0;
    jac(6,2) = 1.0;
    jac(6,4) = 1.0;
    jac(6,5) = 1.0;
    jac(6,6) = 1.0;
  }
};


BOOST_AUTO_TEST_CASE (visualization_gnuplot_differentiable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("visualization-gnuplot-differentiable-function");

  using namespace roboptim::visualization::gnuplot;
  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();

  Square square;
  Square::vector_t arg(7);
  arg.fill(1.0);

  (*output)
    << (gnuplot
	<< plot_jac (square, arg)
	);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
