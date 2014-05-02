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

typedef Function::value_type value_type;
typedef Function::size_type  size_type;

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

  // Test operations on dense matrices
  Eigen::MatrixXd dense_a (5,5);
  Eigen::MatrixXd dense_b (5,5);

  for (int i = 0; i < dense_a.rows (); ++i)
    for (int j = 0; j < dense_a.cols (); ++j)
      {
        dense_a (i,j) = dense_b (i,j) = (double)(i*j);
      }

  BOOST_CHECK (allclose (dense_a, dense_b));

  // Test operations on sparse matrices
  Eigen::SparseMatrix<double> sparse_a (5,5);
  Eigen::SparseMatrix<double> sparse_b (5,5);

  for (int i = 0; i < sparse_a.rows (); ++i)
    for (int j = 0; j < sparse_a.cols (); ++j)
      {
        sparse_a.insert (i,j) = (double)(i*j);
        sparse_b.insert (i,j) = (double)(i*j);
      }

  BOOST_CHECK (allclose (sparse_a, sparse_b));

  const size_type vec_size = 4;
  Function::vector_t eigen_vec (vec_size);
  std::vector<value_type> stl_vec (vec_size);
  eigen_vec.setRandom ();

  // Copy Eigen vector to C array
  detail::vector_to_array (stl_vec.data (), eigen_vec);

  for (size_type i = 0; i < vec_size; ++i)
    BOOST_CHECK_CLOSE (eigen_vec[i], stl_vec[i], 1e-6);

  // Copy C array to Eigen vector
  Function::vector_t eigen_vec2 (vec_size);
  detail::array_to_vector (eigen_vec2, stl_vec.data ());
  BOOST_CHECK (allclose (eigen_vec, eigen_vec2));

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
