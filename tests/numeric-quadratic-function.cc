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
#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

typedef DummySolver solver_t;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (numeric_quadratic_function, T, functionTypes_t)
{
  typename GenericNumericQuadraticFunction<T>::matrix_t a (5, 5);
  typename GenericNumericQuadraticFunction<T>::vector_t b (5);
  typename GenericNumericQuadraticFunction<T>::vector_t x (5);

  a.setZero ();
  b.setZero ();
  x.setZero ();

  a.coeffRef (0, 0) = 1., a.coeffRef (0, 1) = 0.,
    a.coeffRef (0, 2) = 0., a.coeffRef (0, 3) = 0., a.coeffRef (0, 4) = 0.;
  a.coeffRef (1, 0) = 0., a.coeffRef (1, 1) = 1.,
    a.coeffRef (1, 2) = 0., a.coeffRef (1, 3) = 0., a.coeffRef (1, 4) = 0.;
  a.coeffRef (2, 0) = 0., a.coeffRef (2, 1) = 0.,
    a.coeffRef (2, 2) = 1., a.coeffRef (2, 3) = 0., a.coeffRef (2, 4) = 0.;
  a.coeffRef (3, 0) = 0., a.coeffRef (3, 1) = 0.,
    a.coeffRef (3, 2) = 0., a.coeffRef (3, 3) = 1., a.coeffRef (3, 4) = 0.;
  a.coeffRef (4, 0) = 0., a.coeffRef (4, 1) = 0.,
    a.coeffRef (4, 2) = 0., a.coeffRef (4, 3) = 0., a.coeffRef (4, 4) = 1.;

  b[0] = 0.;
  b[1] = 0.;
  b[2] = 0.;
  b[3] = 0.;
  b[4] = 0.;

  GenericNumericQuadraticFunction<T> f (a, b);

  std::cout << f << '\n';

  for (int i = 0; i < 10; ++i)
    {
      for (int j = 0; j < 5; ++j)
	x[j] = std::ceil (rand () % 50);

      std::cout << "x = " << x << '\n';
      std::cout << "f(x) = " << f (x) << '\n';
      std::cout << "J(x) = " << f.jacobian (x) << '\n';
      std::cout << "G(x) = " << f.gradient (x, 0) << '\n';
      std::cout << "H(x) = " << f.hessian (x, 0) << '\n';

      typedef typename GenericNumericQuadraticFunction<T>::matrix_t matrix_t;
      matrix_t J (1, 5);

      for (typename matrix_t::Index i = 0; i < 5; ++i)
	J.coeffRef (0, i) = 2 * x[i];

      for (typename matrix_t::Index i = 0; i < 5; ++i)
	{
	  std::cout << f.jacobian (x).coeffRef (0, i) << '\n';
	  std::cout << J.coeffRef (0, i) << '\n';
	}

      BOOST_CHECK (allclose (f.jacobian (x), J));
      BOOST_CHECK (allclose (f.hessian (x, 0), a));

      BOOST_CHECK (checkGradient (f, 0, x));
      BOOST_CHECK (checkJacobian (f, x));
    }
}

typedef boost::mpl::list< ::roboptim::EigenMatrixSparse> sparseOnly_t;

BOOST_AUTO_TEST_CASE_TEMPLATE (random_gradient_check, T, sparseOnly_t)
{
  typename GenericNumericQuadraticFunction<T>::matrix_t a (5, 5);
  typename GenericNumericQuadraticFunction<T>::vector_t b (5);
  typename GenericNumericQuadraticFunction<T>::vector_t x (5);

  for (int randomTry = 0; randomTry < 10; ++randomTry)
    {
      a.setZero ();
      b.setZero ();
      x.setZero ();

      for (typename GenericNumericQuadraticFunction<T>::matrix_t::Index i = 0; i < 5; ++i)
	for (typename GenericNumericQuadraticFunction<T>::matrix_t::Index j = 0; j < 5; ++j)
	  a.insert (i, j) = 0.;
      for (typename GenericNumericQuadraticFunction<T>::matrix_t::Index i = 0; i < 5; ++i)
	for (typename GenericNumericQuadraticFunction<T>::matrix_t::Index j = 0; j < 5; ++j)
	  a.coeffRef (i, j) = a.coeffRef (j, i) = static_cast<double> (std::rand () / RAND_MAX);

      b = GenericNumericQuadraticFunction<T>::vector_t::Random (5);

      GenericNumericQuadraticFunction<T> f (a, b);

      for (int i = 0; i < 10; ++i)
	{
	  for (int j = 0; j < 5; ++j)
	    x[j] = std::ceil (rand () % 50);

	  BOOST_CHECK (checkGradient (f, 0, x));
	  BOOST_CHECK (checkJacobian (f, x));
	}
    }
}


BOOST_AUTO_TEST_SUITE_END ()
