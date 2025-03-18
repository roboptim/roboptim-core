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

#include <boost/test/unit_test.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/operator/selection.hh>

#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/identity.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (chain_test, T, functionTypes_t)
{
  typename GenericIdentityFunction<T>::result_t offset (5);
  offset.setZero ();

  boost::shared_ptr<GenericIdentityFunction<T> > identity =
    boost::make_shared<GenericIdentityFunction<T> > (offset);

  boost::shared_ptr<GenericLinearFunction<T> >
    fct = selection (identity, 0, 1);

  BOOST_CHECK (fct->outputSize () == 1);

  typename GenericIdentityFunction<T>::vector_t x (5);
  x.setZero ();
  std::cout
    << (*fct) (x) << "\n"
    << fct->gradient (x, 0) << "\n"
    << fct->jacobian (x) << std::endl;

  BOOST_CHECK_THROW (fct = selection (identity, 0, 10), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END ()
