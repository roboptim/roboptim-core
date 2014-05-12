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

using namespace roboptim;
using namespace roboptim::visualization;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (visualization_gnuplot_simple)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("visualization-gnuplot-simple");

  using namespace roboptim::visualization::gnuplot;
  Gnuplot gnuplot = Gnuplot::make_gnuplot ();

  // Display nothing, generate just some valid Gnuplot commands.
  (*output)
    << (gnuplot
	<< set ("term", "x11 enhanced persist")
	<< comment ("Hello, world!")
	<< cd ("/tmp")
	<< help ("pwd")
	<< show ("foo")
	<< replot
	<< pwd
	<< reread
	<< clear
	<< reset
	<< quit
	);

  // Test clear function
  gnuplot.clear ();
  (*output) << gnuplot;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
