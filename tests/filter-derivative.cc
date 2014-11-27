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
#include <roboptim/core/filter/derivative.hh>

#include <roboptim/core/function/cos.hh>
#include <roboptim/core/function/identity.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (derivative_test, T, functionTypes_t)
{
  boost::shared_ptr<Cos<T> > cos =
    boost::make_shared<Cos<T> > ();
  boost::shared_ptr<GenericDifferentiableFunction<T> > fct =
    derivative (cos, 0);

  typename Cos<T>::argument_t x (1);
  x.setZero ();
  std::cout
    << "Input: " << (*cos) << std::endl
    << "Output: " << (*fct) << std::endl
    << "Evaluate:" << std::endl
    << (*fct) (x) << std::endl
    << "Gradient:" << std::endl
    << fct->gradient (x, 0) << std::endl
    << "Jacobian:" << std::endl
    << fct->jacobian (x) << std::endl;
}

BOOST_AUTO_TEST_CASE_TEMPLATE (nonscalar_derivative_test, T, functionTypes_t)
{
  typename GenericIdentityFunction<T>::vector_t offset (3);
  offset.setZero ();
  boost::shared_ptr<GenericIdentityFunction<T> > id =
    boost::make_shared<GenericIdentityFunction<T> > (offset);
  boost::shared_ptr<GenericDifferentiableFunction<T> > fct =
    derivative (id, 0);

  typename GenericFunction<T>::argument_t x (offset.size ());
  x.setZero ();
  std::cout
    << "Input: " << (*id) << std::endl
    << "Output: " << (*fct) << std::endl
    << "Evaluate:" << std::endl
    << (*fct) (x) << std::endl
    << "Gradient:" << std::endl
    << fct->gradient (x, 0) << std::endl
    << "Jacobian:" << std::endl
    << fct->jacobian (x) << std::endl;
}


BOOST_AUTO_TEST_SUITE_END ()
