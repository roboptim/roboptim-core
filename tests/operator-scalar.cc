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
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/detail/autopromote.hh>

using namespace roboptim;


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

template <typename T>
class F : public GenericDifferentiableFunction<T>
{
public:
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  explicit F ();
  void impl_compute (result_ref result, const_argument_ref x) const;
  void impl_gradient (gradient_ref grad, const_argument_ref x, size_type) const;
};

template <typename T>
F<T>::F () : GenericDifferentiableFunction<T> (1, 1, "1 + x²")
{}

template <typename T>
void F<T>::impl_compute (result_ref result, const_argument_ref x) const
{
  result[0] = 1. + x[0] * x[0];
}

template <>
void F<EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type) const
{
  grad.insert (0) = 2. * x[0];
}

template <typename T>
void F<T>::impl_gradient (gradient_ref grad, const_argument_ref x, size_type) const
{
  grad[0] = 2. * x[0];
}

// Register F for autopromote support
ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
(F<EigenMatrixDense>,
 GenericDifferentiableFunction<EigenMatrixDense>)
ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
(F<EigenMatrixSparse>,
 GenericDifferentiableFunction<EigenMatrixSparse>)


BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (scalar_test, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("operator-scalar");

  // Test for differentiable functions
  {
    typedef F<T> testFunction_t;
    typedef GenericDifferentiableFunction<T> testPromoteFunction_t;

    boost::shared_ptr<testFunction_t >
      f = boost::make_shared<testFunction_t > ();

    boost::shared_ptr<testPromoteFunction_t> g = 2. * f;

    typename testFunction_t::vector_t x (1);
    x.fill (1.);
    (*output)
      << x << "\n"
      << (*f) << "\n"
      << (*f) (x) << "\n"
      << toDense (f->gradient (x, 0)) << "\n"
      << toDense (f->jacobian (x)) << "\n"
      << (*g) << "\n"
      << (*g) (x) << "\n"
      << toDense (g->gradient (x, 0)) << "\n"
      << toDense (g->jacobian (x)) << std::endl;
  }

  // Test for twice-differentiable functions
  {
    typedef GenericNumericQuadraticFunction<T> testFunction_t;
    typedef GenericQuadraticFunction<T> testPromoteFunction_t;
    typedef typename testFunction_t::size_type size_type;

    size_type n = 4;
    typename testFunction_t::matrix_t a (n, n);
    a.setZero ();
    for (size_type i = 0; i < n; ++i)
      a.coeffRef (i, i) = 1.;
    typename testFunction_t::vector_t b (n);
    b.setZero ();

    boost::shared_ptr<testFunction_t >
      f = boost::make_shared<testFunction_t > (a, b);

    boost::shared_ptr<testPromoteFunction_t> g = 2. * f;

    typename testFunction_t::vector_t x (n);
    x.fill (1.);
    (*output)
      << x << "\n"
      << (*f) << "\n"
      << (*f) (x) << "\n"
      << toDense (f->gradient (x, 0)) << "\n"
      << toDense (f->jacobian (x)) << "\n"
      << (*g) << "\n"
      << (*g) (x) << "\n"
      << toDense (g->gradient (x, 0)) << "\n"
      << toDense (g->jacobian (x)) << "\n"
      << toDense (g->hessian (x)) << std::endl;
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
