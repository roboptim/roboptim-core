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
#include <roboptim/core/function/sin.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (sin_test, T, functionTypes_t)
{
  boost::shared_ptr<Sin<T> > fct =
    boost::make_shared<Sin<T> > ();

  typename Sin<T>::argument_t x (1);
  x.setZero ();
  std::cout
    << (*fct) << std::endl
    << "Evaluate:" << std::endl
    << (*fct) (x) << std::endl
    << "Gradient:" << std::endl
    << fct->gradient (x, 0) << std::endl
    << "Jacobian:" << std::endl
    << fct->jacobian (x) << std::endl
    << "Hessian:" << std::endl
    << fct->hessian (x) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END ()
