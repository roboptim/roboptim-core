// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

#include <roboptim/core/function.hh>
#include <roboptim/core/visualization/util.hh>

using namespace roboptim;
using namespace roboptim::visualization;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (visualization_util)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("visualization-util");

  typedef Function::size_type size_type;

  Function::vector_t v (10);
  for (size_type i = 0; i < v.size (); ++i)
    {
      v[i] = std::pow (10., -i);
    }

  (*output) << v << std::endl
            << normalize (v) << std::endl;

  Function::matrix_t m (4,4);
  for (size_type i = 0; i < m.rows (); ++i)
    for (size_type j = 0; j < m.cols (); ++j)
      {
	m(i,j) = std::pow (10., -(m.cols () * i + j));
      }

  (*output) << m << std::endl
            << normalize (m) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
