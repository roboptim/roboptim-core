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
#include <roboptim/core/identity-function.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (identity_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("identity-function");

  IdentityFunction::vector_t offset (4);
  offset[0] = 12.;
  offset[1] = 46.;
  offset[2] = 2.;
  offset[3] = -9.;

  IdentityFunction id (offset);

  IdentityFunction::vector_t x (4);
  x.setZero ();

  (*output)
    << id << std::endl
    << "Evaluate: " << std::endl
    << id (x) << std::endl
    << "Gradient: " << std::endl
    << id.gradient (x) << std::endl
    << "Jacobian: " << std::endl
    << id.jacobian (x) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
