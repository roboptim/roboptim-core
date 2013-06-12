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

#include "shared-tests/fixture.hh"

#include <iostream>

#include <roboptim/core/io.hh>

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-differentiable-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;


// Define f(x) = forty_two(x)   (dense version)
struct FortyTwoDense : public DifferentiableFunction
{
  explicit FortyTwoDense ()
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


// Define f(x) = forty_two(x)   (sparse version)
struct FortyTwoSparse : public DifferentiableSparseFunction
{
  explicit FortyTwoSparse ()
    : DifferentiableSparseFunction (7, 7, "The Answer")
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
    jac.insert(0,0) = 1.0;
    jac.insert(0,4) = 1.0;
    jac.insert(0,5) = 1.0;
    jac.insert(1,0) = 1.0;
    jac.insert(1,2) = 1.0;
    jac.insert(1,6) = 1.0;
    jac.insert(2,0) = 1.0;
    jac.insert(2,2) = 1.0;
    jac.insert(2,6) = 1.0;
    jac.insert(3,0) = 1.0;
    jac.insert(3,1) = 1.0;
    jac.insert(3,2) = 1.0;
    jac.insert(3,5) = 1.0;
    jac.insert(4,2) = 1.0;
    jac.insert(4,4) = 1.0;
    jac.insert(5,2) = 1.0;
    jac.insert(5,4) = 1.0;
    jac.insert(6,2) = 1.0;
    jac.insert(6,4) = 1.0;
    jac.insert(6,5) = 1.0;
    jac.insert(6,6) = 1.0;
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (visualization_gnuplot_differentiable_function)
{

  using namespace roboptim::visualization::gnuplot;
  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot ();


  // Test #1: dense version
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern("visualization-gnuplot-differentiable-function");

  FortyTwoDense f_dense;
  FortyTwoDense::vector_t arg_dense(7);
  arg_dense.fill(1.0);

  (*output)
    << (gnuplot
	<< plot_jac (f_dense, arg_dense)
	);

  std::cout << output->str () << std::endl;

  // Match pattern and flush output
  BOOST_CHECK (output->match_pattern (true));
  // Reset pattern


  // Test #2: sparse version
  FortyTwoSparse f_sparse;
  FortyTwoSparse::vector_t arg_sparse(7);
  arg_sparse.fill(1.0);

  // Reset gnuplot instance and pattern
  gnuplot = Gnuplot::make_interactive_gnuplot ();
  output = retrievePattern("visualization-gnuplot-differentiable-function");

  (*output)
    << (gnuplot
	<< plot_jac (f_sparse, arg_sparse)
	);

  std::cout << output->str () << std::endl;

  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
