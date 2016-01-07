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

void test_normalization
(boost::shared_ptr<boost::test_tools::output_test_stream> output)
{
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

}

BOOST_AUTO_TEST_CASE (util)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("util");

  // Check vector display.
  std::vector<int> v;

  (*output) << v << std::endl;

  v.push_back (1);
  (*output) << v << std::endl;

  // Check pair display.
  std::pair<int, std::string> p (42, "foo");
  (*output) << p << std::endl;

  // Check map display.
  std::map<std::string, int> m;
  (*output) << m << std::endl;
  m["foo"] = 42;
  m["bar"] = 1337;
  (*output) << m << std::endl;

  // Test typeString
  // TODO: relies on demangle() so may fail on different platforms
  (*output) << typeString<int> () << std::endl;
  (*output) << typeString<Function> () << std::endl;

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

  // Test conversions to dense
  // Note: extra conversion step to solve compilation error in RowMajor mode
  typedef GenericFunctionTraits<EigenMatrixDense>::matrix_t denseMatrix_t;
  typedef GenericFunctionTraits<EigenMatrixSparse>::matrix_t sparseMatrix_t;
  (*output) << toDense (denseMatrix_t (dense_a)) << std::endl;
  (*output) << toDense (sparseMatrix_t (sparse_a)) << std::endl;

  typedef GenericFunctionTraits<EigenMatrixDense>::gradient_t denseGradient_t;
  denseGradient_t dense_grad (4);
  dense_grad << 1., 2., 3., 4.;
  typedef GenericFunctionTraits<EigenMatrixSparse>::gradient_t sparseGradient_t;
  sparseGradient_t sparse_grad (4);
  for (sparseGradient_t::Index i = 0; i < 4; ++i)
    sparse_grad.insert (i) = static_cast<double> (i+1);
  (*output) << toDense (dense_grad) << std::endl;
  (*output) << toDense (sparse_grad) << std::endl;

  // Copy Eigen vector to C array
  detail::vector_to_array (stl_vec.data (), eigen_vec);

  for (size_type i = 0; i < vec_size; ++i)
    BOOST_CHECK_CLOSE (eigen_vec[i],
                       stl_vec[static_cast<size_t> (i)],
                       1e-6);

  // Copy C array to Eigen vector
  Function::vector_t eigen_vec2 (vec_size);
  detail::array_to_vector (eigen_vec2, stl_vec.data ());
  BOOST_CHECK (allclose (eigen_vec, eigen_vec2));

  // Test sparse block copy
  typedef GenericFunctionTraits<EigenMatrixSparse>::matrix_t sparseMatrix_t;
  sparseMatrix_t sparse_c (5,5);
  sparseMatrix_t sparse_d (5,5);
  sparseMatrix_t sparse_block (2,2);
  sparse_block.coeffRef (0, 0) = 42.;
  sparse_block.coeffRef (1, 0) = 2.;
  sparse_block.coeffRef (1, 1) = -1.;
  sparse_c = sparse_a;
  sparse_d = sparse_a;
  BOOST_CHECK (allclose (sparse_c, sparse_d));
  Eigen::SparseMatrix<double>::Index startRow = 2;
  Eigen::SparseMatrix<double>::Index startCol = 1;
  for (Eigen::SparseMatrix<double>::Index i = 0; i < sparse_block.rows (); ++i)
    for (Eigen::SparseMatrix<double>::Index j = 0; j < sparse_block.cols (); ++j)
      sparse_d.coeffRef (startRow + i, startCol + j) = sparse_block.coeffRef (i, j);
  copySparseBlock (sparse_c, sparse_block, startRow, startCol);
  BOOST_CHECK (allclose (sparse_c, sparse_d));
  sparse_c.setZero ();
  sparse_c = sparse_a;
  copySparseBlock (sparse_c, sparse_block, startRow, startCol, true);
  BOOST_CHECK (allclose (sparse_c, sparse_d));

  // Test sparse block update
  sparse_block = sparseMatrix_t (3,3);
  sparse_c.setZero ();
  sparse_d.setZero ();
  sparse_block.coeffRef (0, 0) = 12.;
  sparse_block.coeffRef (0, 2) = 42.;
  sparse_block.coeffRef (1, 2) = 1.;
  sparse_c.coeffRef (0,          startCol)   = 3.;
  sparse_c.coeffRef (startRow,   startCol)   = 4.;
  sparse_c.coeffRef (startRow,   startCol+2) = 5.;
  sparse_c.coeffRef (startRow+1, startCol+2) = 6.;
  sparse_d.coeffRef (0,          startCol)   = 3.;
  sparse_d.coeffRef (startRow,   startCol)   = 12.;
  sparse_d.coeffRef (startRow,   startCol+2) = 42.;
  sparse_d.coeffRef (startRow+1, startCol+2) = 1.;
  (*output) << Eigen::MatrixXd (sparse_d) << std::endl;
  (*output) << Eigen::MatrixXd (sparse_c) << std::endl;
  (*output) << Eigen::MatrixXd (sparse_block) << std::endl;
  BOOST_CHECK_NO_THROW (updateSparseBlock (sparse_c, sparse_block,
                                           startRow, startCol));
  (*output) << Eigen::MatrixXd (sparse_c) << std::endl;
  BOOST_CHECK (allclose (sparse_c, sparse_d));
  sparseMatrix_t sparse_empty (sparse_c.rows (), sparse_c.cols ());
  BOOST_CHECK_THROW (updateSparseBlock (sparse_empty, sparse_block, 0, 0),
                     std::runtime_error);
  sparseMatrix_t sparse_scaled = sparse_c * 2.;
  BOOST_CHECK_NO_THROW (updateSparseBlock (sparse_c,
                                           sparseMatrix_t (sparse_scaled.block (0,0,5,5)),
                                           0, 0));
  (*output) << Eigen::MatrixXd (sparse_c) << std::endl;
  BOOST_CHECK (allclose (sparse_c, sparse_scaled));
  sparse_scaled = sparse_c * 2.;
  BOOST_CHECK_NO_THROW (updateSparseBlock (sparse_c,
                                           sparse_scaled.block (0,0,5,5),
                                           0, 0));
  (*output) << Eigen::MatrixXd (sparse_c) << std::endl;
  BOOST_CHECK (allclose (sparse_c, sparse_scaled));

  // Test normalization
  test_normalization (output);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
