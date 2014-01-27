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
#include <boost/format.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/filter/chain.hh>
#include <roboptim/core/filter/selection.hh>

#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/identity.hh>

using namespace roboptim;


// FIXME: sparse matrices not supported yet.
// ::roboptim::EigenMatrixSparse
typedef boost::mpl::list< ::roboptim::EigenMatrixDense> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (chain_test, T, functionTypes_t)
{
  typename GenericIdentityFunction<T>::result_t offset5 (5);
  typename GenericIdentityFunction<T>::result_t offset1 (1);
  offset1.setZero ();
  offset5.setZero ();

  // f(x) = x
  // input: 1
  // output: 1
  boost::shared_ptr<GenericIdentityFunction<T> > identity =
    boost::make_shared<GenericIdentityFunction<T> > (offset1);

  // g(x) = 0
  // input: 5
  // output: 5
  boost::shared_ptr<GenericConstantFunction<T> > constant =
    boost::make_shared<GenericConstantFunction<T> > (offset5);

  // g2(x) = 0
  // input: 5
  // output: 1
  boost::shared_ptr<GenericLinearFunction<T> > selec_constant
          = roboptim::selection (constant, 2, 1);

  boost::format fmt("%s input %d output %d");
  std::cout
    << (fmt
	% "identity"
	% identity->inputSize ()
	% identity->outputSize ())
    << "\n";
  std::cout
    << (fmt
	% "selec_constant"
	% selec_constant->inputSize ()
	% selec_constant->outputSize ())
    << std::endl;

  // f(g2(x))
  // input: 5
  // output: 1
  boost::shared_ptr<GenericLinearFunction<T> >
    fct = chain (identity, selec_constant);

  typename GenericIdentityFunction<T>::argument_t x (5);
  x.setZero ();
  std::cout
    << (*fct) (x) << "\n"
    << fct->gradient (x, 0) << "\n"
    << fct->jacobian (x) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END ()
