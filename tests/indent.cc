// Copyright (C) 2014 by Benjamin Chrétien, CNRS.
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
#include <roboptim/core/indent.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (indent)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("indent");

  (*output) << "Test 1" << incindent;
  (*output) << iendl << "Dummy 1";
  (*output) << iendl << "Dummy 2";
  (*output) << incindent;
  (*output) << iendl << "Foo";
  (*output) << iendl << "Bar";
  (*output) << decindent;
  (*output) << iendl << "Dummy 3";
  (*output) << resetindent << iendl << "Test 2";
  (*output) << incendl << "Dummy 4";
  (*output) << decendl << "Test 3";

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
