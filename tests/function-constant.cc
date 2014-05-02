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
#include <roboptim/core/function/constant.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (constant_function, T, functionTypes_t)
{
  typename GenericConstantFunction<T>::vector_t offset (4);
  offset[0] = 12.;
  offset[1] = 46.;
  offset[2] = 2.;
  offset[3] = -9.;

  GenericConstantFunction<T> cst (offset);

  typename GenericConstantFunction<T>::vector_t x (4);
  x.setZero ();

  std::cout
    << cst << std::endl
    << "Evaluate:" << std::endl
    << cst (x) << std::endl
    << "Gradient:" << std::endl
    << cst.gradient (x) << std::endl
    << "Jacobian:" << std::endl
    << cst.jacobian (x) << std::endl
    << "Hessian:" << std::endl
    << cst.hessian (x) << std::endl;

  typename GenericConstantFunction<T>::vector_t result = cst (x);
  BOOST_CHECK (allclose (offset, result));
}

BOOST_AUTO_TEST_SUITE_END ()
