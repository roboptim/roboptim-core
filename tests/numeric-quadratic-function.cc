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
#include <roboptim/core/numeric-quadratic-function.hh>

using namespace roboptim;

typedef DummySolver solver_t;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (numeric_linear_function, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("numeric-quadratic-function");

  typename GenericNumericQuadraticFunction<T>::matrix_t a (5, 5);
  typename GenericNumericQuadraticFunction<T>::vector_t b (5);
  typename GenericNumericQuadraticFunction<T>::vector_t x (5);

  a.setZero ();
  b.setZero ();
  x.setZero ();

  a.coeffRef (0, 0) = 1.1, a.coeffRef (0, 1) = 1.2, a.coeffRef (0, 2) = 1.3, a.coeffRef (0, 3) = 1.4, a.coeffRef (0, 4) = 1.5;
  a.coeffRef (1, 0) = 1.2, a.coeffRef (1, 1) = 2.2, a.coeffRef (1, 2) = 2.3, a.coeffRef (1, 3) = 2.4, a.coeffRef (1, 4) = 2.5;
  a.coeffRef (2, 0) = 1.3, a.coeffRef (2, 1) = 2.3, a.coeffRef (2, 2) = 3.3, a.coeffRef (2, 3) = 3.4, a.coeffRef (2, 4) = 3.5;
  a.coeffRef (3, 0) = 1.4, a.coeffRef (3, 1) = 2.4, a.coeffRef (3, 2) = 3.4, a.coeffRef (3, 3) = 4.4, a.coeffRef (3, 4) = 4.5;
  a.coeffRef (4, 0) = 1.5, a.coeffRef (4, 1) = 2.5, a.coeffRef (4, 2) = 3.5, a.coeffRef (4, 3) = 4.5, a.coeffRef (4, 4) = 5.5;

  b[0] = 2.1;
  b[1] = 4.3;
  b[2] = 6.5;
  b[3] = 8.7;

  GenericNumericQuadraticFunction<T> f (a, b);

  (*output) << f << std::endl;

  x[0] = 0.1;
  x[1] = 1.2;
  x[2] = 2.3;
  x[3] = 3.4;
  x[4] = 4.5;

  (*output) << "f(x) = " << f (x) << std::endl;
  (*output) << "J(x) = " << f.jacobian (x) << std::endl;
  (*output) << "G(x) = " << f.gradient (x, 0) << std::endl;
  (*output) << "H(x) = " << f.hessian (x, 0) << std::endl;

  std::cout << output->str () << std::endl;
  //BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
