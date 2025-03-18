// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#include <iostream>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/function/polynomial.hh>

#include "shared-tests/fixture.hh"

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (polynomial_test, T, functionTypes_t)
{
  // Test for degree 0 polynomial
  typename Polynomial<T>::vector_t coefficients (1);
  coefficients[0] = 45;
  boost::shared_ptr<Polynomial<T> > fct =
    boost::make_shared<Polynomial<T> > (coefficients);
  std::cout << (*fct) << std::endl;

  typename Polynomial<T>::argument_t x (1);
  x[0] = 0;
  typename Polynomial<T>::result_t y (1);
  y[0] = 45;
  BOOST_CHECK_EQUAL ((*fct) (x), y);

  x[0] = 1;
  y[0] = 45;
  BOOST_CHECK_EQUAL ((*fct) (x), y);

  // Test for degree 1 polynomial
  coefficients.resize (2);
  coefficients[0] = 3;
  coefficients[1] = 5;
  fct = boost::make_shared<Polynomial<T> > (coefficients);
  std::cout << (*fct) << std::endl;

  x[0] = 0;
  y[0] = 3;
  BOOST_CHECK_EQUAL ((*fct) (x), y);
  x[0] = 1;
  y[0] = 8;
  BOOST_CHECK_EQUAL((*fct) (x), y);

  // Test for degree 2 polynomial
  coefficients.resize (3);
  coefficients[0] = 3;
  coefficients[1] = 5;
  coefficients[2] = 2;
  fct = boost::make_shared<Polynomial<T> > (coefficients);
  std::cout << (*fct) << std::endl;

  x[0] = 0;
  y[0] = 3;
  BOOST_CHECK_EQUAL ((*fct) (x), y);
  x[0] = 1;
  y[0] = 10;
  BOOST_CHECK_EQUAL ((*fct) (x), y);
  x[0] = 3;
  y[0] = 36;
  BOOST_CHECK_EQUAL ((*fct) (x), y);

  // Test for degree 3 polynomial
  coefficients.resize (4);
  coefficients[0] = 3;
  coefficients[1] = 5;
  coefficients[2] = 2;
  coefficients[3] = 8;
  fct = boost::make_shared<Polynomial<T> > (coefficients);
  std::cout << (*fct) << std::endl;

  x[0] = 0;
  y[0] = 3;
  BOOST_CHECK_EQUAL ((*fct) (x), y);
  x[0] = 1;
  y[0] = 18;
  BOOST_CHECK_EQUAL ((*fct) (x), y);
  x[0] = 3;
  y[0] = 252;
  BOOST_CHECK_EQUAL ((*fct) (x), y);

  // Test the usual function methods
  x.fill (1.);
  std::cout << std::endl
	    << (*fct) << std::endl
	    << "Evaluate:" << std::endl
	    << (*fct) (x) << std::endl
	    << "Gradient:" << std::endl
	    << fct->gradient (x, 0) << std::endl
	    << "Jacobian:" << std::endl
	    << fct->jacobian (x) << std::endl
	    << "Hessian:" << std::endl
	    << fct->hessian (x) << std::endl;

  // Test exceptions
  coefficients.resize (0);
  BOOST_CHECK_THROW (fct = boost::make_shared<Polynomial<T> > (coefficients),
                     std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END ()
