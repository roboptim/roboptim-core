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
#include <roboptim/core/util.hh>
#include <roboptim/core/operator/scalar.hh>
#include <roboptim/core/function/identity.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (scalar_test, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("operator-scalar");

  typename GenericIdentityFunction<T>::result_t offset (5);
  offset << 1., 2., 0., 4., 5.;

  boost::shared_ptr<GenericIdentityFunction<T> > identity =
    boost::make_shared<GenericIdentityFunction<T> > (offset);

  boost::shared_ptr<GenericLinearFunction<T> > fct = 2. * identity;

  typename GenericIdentityFunction<T>::vector_t x (5);
  x.setZero ();
  (*output)
    << (*fct) << "\n"
    << (*fct) (x) << "\n"
    << toDense (fct->gradient (x, 0)) << "\n"
    << toDense (fct->gradient (x, 1)) << "\n"
    << toDense (fct->gradient (x, 2)) << "\n"
    << toDense (fct->gradient (x, 3)) << "\n"
    << toDense (fct->gradient (x, 4)) << "\n"
    << toDense (fct->jacobian (x)) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
