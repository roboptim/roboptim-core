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
#include <roboptim/core/filter/concatenate.hh>

#include <roboptim/core/function/cos.hh>
#include <roboptim/core/function/sin.hh>
#include <roboptim/core/function/constant.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (concatenate_test, T, functionTypes_t)
{
  boost::shared_ptr<Cos<T> > cosinus = boost::make_shared<Cos<T> > ();
  boost::shared_ptr<Sin<T> > sinus = boost::make_shared<Sin<T> > ();

  boost::shared_ptr<GenericDifferentiableFunction<T> >
    fct = concatenate (cosinus, sinus);

  BOOST_CHECK (fct->outputSize () == 1 + 1);

  typename GenericDifferentiableFunction<T>::argument_t x (1);
  x.setZero ();
  std::cout
    << (*fct) (x) << "\n"
    << fct->gradient (x, 0) << "\n"
    << fct->gradient (x, 1) << "\n"
    << fct->jacobian (x) << std::endl;

  typename GenericConstantFunction<T>::result_t offset1 (2);
  offset1.setZero ();
  typename GenericConstantFunction<T>::result_t offset2 (4);
  offset2.setZero ();
  boost::shared_ptr<GenericConstantFunction<T> > constant1 =
    boost::make_shared<GenericConstantFunction<T> > (offset1);
  boost::shared_ptr<GenericConstantFunction<T> > constant2 =
    boost::make_shared<GenericConstantFunction<T> > (offset2);
  BOOST_CHECK_THROW (fct = concatenate (constant1, constant2),
                     std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END ()
