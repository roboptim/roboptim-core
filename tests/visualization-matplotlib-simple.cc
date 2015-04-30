// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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

using namespace roboptim;
using namespace roboptim::visualization;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (visualization_matplotlib_simple)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("visualization-matplotlib-simple");

  using namespace roboptim::visualization::matplotlib;
  Matplotlib matplotlib = Matplotlib::make_matplotlib (std::make_pair(1, 1), false);

  // Display nothing, generate just some valid matplotlib commands.
  (*output)
    << (matplotlib
	<< set ("foo", "42")
	<< comment ("Hello, world!\nThis is a three-line comment!\nAmazing!")
	<< import ("foo")
	<< import ("foo", "bar")
	<< figure ()
	<< show ()
	);

  // Test clear function.
  matplotlib.clear ();
  // Enable header.
  matplotlib.withHeader () = true;
  BOOST_CHECK (matplotlib.withHeader () == true);
  (*output) << std::endl << matplotlib;

  // Test some throws.
  BOOST_CHECK_THROW ((*output) << (matplotlib << set ("bar", 0)),
                     std::runtime_error);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
