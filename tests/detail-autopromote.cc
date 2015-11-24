// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>

#include <roboptim/core/fwd.hh>
#include <roboptim/core/linear-function.hh>
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/quadratic-function.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/cos.hh>
#include <roboptim/core/function/identity.hh>
#include <roboptim/core/function/sin.hh>
#include <roboptim/core/operator/plus.hh>
#include <roboptim/core/operator/scalar.hh>

#include <roboptim/core/detail/autopromote.hh>


using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

// Test a predicate at compile-time and run-time.
#define TEST_PREDICATE(PRED) \
  BOOST_MPL_ASSERT ((PRED)); \
  BOOST_CHECK ((PRED::type::value));

#define TEST_AUTOPROMOTE_TRAIT(U,V)                                       \
  {                                                                       \
    typedef is_same<typename AutopromoteTrait<U>::T_type, V> predicate_t; \
    TEST_PREDICATE (predicate_t);                                         \
  }


BOOST_AUTO_TEST_CASE_TEMPLATE (detail_autopromote, T, functionTypes_t)
{
  using namespace boost;
  using namespace boost::mpl;
  using namespace ::roboptim::detail;

  // Test AutopromoteTrait
  {
    // Functions
    TEST_AUTOPROMOTE_TRAIT (GenericNumericQuadraticFunction<T>,
                            GenericQuadraticFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (GenericNumericLinearFunction<T>,
                            GenericLinearFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (GenericConstantFunction<T>,
                            GenericLinearFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (GenericIdentityFunction<T>,
                            GenericLinearFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (Cos<T>,
                            GenericDifferentiableFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (Sin<T>,
                            GenericDifferentiableFunction<T>);

    // Operators
    TEST_AUTOPROMOTE_TRAIT (Plus<Cos<T> BOOST_PP_COMMA()
                                 GenericConstantFunction<T> >,
                            GenericDifferentiableFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (Plus<GenericLinearFunction<T> BOOST_PP_COMMA()
                                 GenericConstantFunction<T> >,
                            GenericLinearFunction<T>);

    TEST_AUTOPROMOTE_TRAIT (Scalar<Cos<T> >,
                            GenericDifferentiableFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (Scalar<GenericLinearFunction<T> >,
                            GenericLinearFunction<T>);
    TEST_AUTOPROMOTE_TRAIT (Scalar<GenericNumericQuadraticFunction<T> >,
                            GenericQuadraticFunction<T>);

    // Extra
    TEST_AUTOPROMOTE_TRAIT (GenericLinearFunction<T>,
                            GenericLinearFunction<T>);
  }
}

BOOST_AUTO_TEST_SUITE_END ()
