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
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

typedef DummySolver solver_t;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (numeric_linear_function, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("numeric-linear-function");

  typename GenericNumericLinearFunction<T>::matrix_t a (1, 5);
  typename GenericNumericLinearFunction<T>::vector_t b (1);
  typename GenericNumericLinearFunction<T>::vector_t x (5);

  a.setZero ();
  b.setZero ();
  x.setZero ();

  a.coeffRef (0, 0) = 1.2;
  a.coeffRef (0, 1) = 3.4;
  a.coeffRef (0, 2) = 5.6;
  a.coeffRef (0, 3) = 7.8;

  b[0] = 1.;

  GenericNumericLinearFunction<T> f (a, b, "Dummy");

  (*output) << f << std::endl;

  x[0] = 0.1;
  x[1] = 1.2;
  x[2] = 2.3;
  x[3] = 3.4;
  x[4] = 4.5;

  (*output) << "f(x) = " << f (x) << std::endl;
  (*output) << "G(x) = " << toDense (f.gradient (x, 0)) << std::endl;
  (*output) << "J(x) = " << toDense (f.jacobian (x)) << std::endl;
  (*output) << "H(x) = " << toDense (f.hessian (x)) << std::endl;

  GenericNumericLinearFunction<T> numericLinearFunction (a, b);
  GenericLinearFunction<T>* linearFunction = &numericLinearFunction;

  GenericNumericLinearFunction<T> numericLinearFunctionRebuilt (*linearFunction);

  BOOST_CHECK (allclose (numericLinearFunction.A (),
			 numericLinearFunctionRebuilt.A ()));
  BOOST_CHECK_EQUAL (numericLinearFunction.b (), numericLinearFunctionRebuilt.b ());

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
