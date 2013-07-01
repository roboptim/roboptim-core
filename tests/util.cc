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

#define BOOST_TEST_MODULE util

#include "shared-tests/fixture.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (util)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("util");

  // Check vector display.
  std::vector<int> v;

  (*output) << v << std::endl;

  v.push_back (1);
  (*output) << v << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());

  // Test operations on dense matrices
  Eigen::MatrixXd dense_a(5,5);
  Eigen::MatrixXd dense_b(5,5);

  for (int i = 0; i < dense_a.rows(); ++i)
    for (int j = 0; j < dense_a.cols(); ++j)
      {
        dense_a(i,j) = dense_b(i,j) = (double)(i*j);
      }

  BOOST_CHECK (allclose(dense_a, dense_b));

  // Test operations on sparse matrices
  Eigen::SparseMatrix<double> sparse_a(5,5);
  Eigen::SparseMatrix<double> sparse_b(5,5);

  for (int i = 0; i < sparse_a.rows(); ++i)
    for (int j = 0; j < sparse_a.cols(); ++j)
      {
        sparse_a.insert(i,j) = (double)(i*j);
        sparse_b.insert(i,j) = (double)(i*j);
      }

  BOOST_CHECK (allclose(sparse_a, sparse_b));
}

BOOST_AUTO_TEST_SUITE_END ()
