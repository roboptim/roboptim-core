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
#include <roboptim/core/result.hh>
#include <roboptim/core/result-with-warnings.hh>

using namespace roboptim;

// Define a simple function.
struct F : public Function
{
  F () : Function (1, 1, "x")
  {}

  void impl_compute (result_t& res,
                     const argument_t& x) const
  {
    res (0) = x[0];
  }

  // No gradient, hessian.
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (result)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("result");

  // Instantiate some results.
  Result result (3, 8);
  F f;
  result.constraints.resize (4);
  result.constraints.setZero ();
  result.lambda.resize (4);
  result.lambda.setZero ();

  assert (result.x.size () == 3);
  assert (result.value.size () == 8);
  assert (result.lambda.size () == 0);

  Result result2 (result);
  Result result3 = result;

  (*output) << result << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
