// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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
#include <roboptim/core/cache.hh>

using namespace roboptim;

// Output stream
boost::shared_ptr<boost::test_tools::output_test_stream> output;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (cache)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("cache");

  size_t cache_size = 5;
  LRUCache<std::string, float> cache (cache_size);

  (*output) << cache << std::endl;

  cache.insert ("x₀", 42.);
  (*output) << cache << std::endl;
  cache.insert ("x₁", 1.);
  cache.insert ("x₀", 0.);
  cache.insert ("x₂", 20.);
  cache.insert ("x₂", 2.);
  cache.insert ("x₃", 3.);
  cache.insert ("x₄", 4.);

  // Note: we do not print when the cache has multiple
  // elements since the print order is not deterministic,
  // thus it cannot be used for tests.

  // x₅ overwrites x₁'s cached value (LRU)
  cache.insert ("x₅", 5.);

  (*output) << "x₀ = " << cache["x₀"] << std::endl;
  (*output) << "x₂ = " << cache["x₂"] << std::endl;
  (*output) << "x₃ = " << cache["x₃"] << std::endl;
  (*output) << "x₄ = " << cache["x₄"] << std::endl;
  (*output) << "x₅ = " << cache["x₅"] << std::endl;

  // /!\ x₁ no longer in cache, so a new element is added,
  // and one removed (x₀)
  (*output) << "x₁ = " << cache["x₁"] << std::endl;

  cache.clear ();
  (*output) << cache << std::endl;

  cache.insert ("x₀", 37.);
  (*output) << cache << std::endl;

  // Test for heap memory
  // #1: fixed size matrices
  typedef Eigen::Matrix<double, 100, 100> matrix_t;
  LRUCache<int,matrix_t> matrix_cache (cache_size);

  for (int i = 0; i < 500; ++i)
  {
    matrix_cache.insert (i, matrix_t::Zero ());
    BOOST_CHECK (matrix_cache[i].isZero ());
  }

  // #2: dynamic size matrices
  typedef Eigen::MatrixXd dynMatrix_t;
  LRUCache<int,dynMatrix_t> dynMatrix_cache (cache_size);

  for (int i = 0; i < 500; ++i)
  {
    dynMatrix_cache.insert (i, dynMatrix_t::Zero (100, 100));
    BOOST_CHECK (dynMatrix_cache[i].isZero ());
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()

