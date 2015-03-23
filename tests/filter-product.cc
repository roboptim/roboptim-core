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

#include <boost/mpl/list.hpp>

#include "shared-tests/fixture.hh"

#include <boost/test/test_case_template.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/operator/product.hh>

#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/identity.hh>
#include <roboptim/core/finite-difference-gradient.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (product_test, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("filter-product");

  typename GenericFunction<T>::value_type eps = 1e-6;

  typename GenericIdentityFunction<T>::result_t offset (5);

  offset.setZero ();
  boost::shared_ptr<GenericIdentityFunction<T> > identity =
    boost::make_shared<GenericIdentityFunction<T> > (offset);
  offset.fill (2.);
  boost::shared_ptr<GenericConstantFunction<T> > constant =
    boost::make_shared<GenericConstantFunction<T> > (offset);

  boost::shared_ptr<GenericLinearFunction<T> >
    fct = identity * constant;
  GenericFiniteDifferenceGradient<T> fd_fct (*fct);

  typename GenericIdentityFunction<T>::argument_t x (5);
  for (Function::size_type i = 0; i < x.size (); ++i)
    x[i] = (double)i;

  std::cout
    << "x = " << x << "\n"
    << "U(x) = x = " << (*identity) (x) << "\n"
    << "V(x) = 2 = " << (*constant) (x) << "\n"
    << "U(x) * V(x) = 2x = " << (*fct) (x) << "\n"
    << Function::vector_t (fct->gradient (x, 0)) << "\n"
    << Function::vector_t (fct->gradient (x, 1)) << "\n"
    << Function::vector_t (fct->gradient (x, 2)) << "\n"
    << Function::vector_t (fct->gradient (x, 3)) << "\n"
    << Function::vector_t (fct->gradient (x, 4)) << "\n"
    << Function::matrix_t (fct->jacobian (x)) << std::endl;

  BOOST_CHECK (allclose (Function::matrix_t (fct->jacobian (x)),
                         Function::matrix_t (fd_fct.jacobian (x)),
                         eps, eps));

  std::cout << std::endl;

  typename GenericIdentityFunction<T>::result_t offset_throw (6);
  offset_throw.setZero ();
  boost::shared_ptr<GenericConstantFunction<T> > constant_throw =
    boost::make_shared<GenericConstantFunction<T> > (offset_throw);
  BOOST_CHECK_THROW (fct = identity * constant_throw, std::runtime_error);

  // Test gradient with finite differences
  boost::shared_ptr<GenericDifferentiableFunction<T> >
    fct2 = fct * fct;
  GenericFiniteDifferenceGradient<T> fd_fct2 (*fct2);

  std::cout
    << "x = " << x << "\n"
    << "U(x) = 2x = " << (*fct) (x) << "\n"
    << "V(x) = 2x = " << (*fct) (x) << "\n"
    << "U(x) * V(x) = 4x² = " << (*fct2) (x) << "\n"
    << Function::vector_t (fct2->gradient (x, 0)) << "\n"
    << Function::vector_t (fct2->gradient (x, 1)) << "\n"
    << Function::vector_t (fct2->gradient (x, 2)) << "\n"
    << Function::vector_t (fct2->gradient (x, 3)) << "\n"
    << Function::vector_t (fct2->gradient (x, 4)) << "\n"
    << Function::matrix_t (fct2->jacobian (x)) << std::endl;

  BOOST_CHECK (allclose (Function::matrix_t (fct2->jacobian (x)),
                         Function::matrix_t (fd_fct2.jacobian (x)),
                         eps, eps));

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
