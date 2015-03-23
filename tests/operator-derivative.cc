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
#include <roboptim/core/operator/derivative.hh>

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

template <typename T>
class F : public GenericDifferentiableFunction<T>
{
public:
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  explicit F ();
  void
  impl_compute (result_ref result, const_argument_ref x) const;
  void
  impl_gradient (gradient_ref grad, const_argument_ref x, size_type)
    const;
};

template <typename T>
F<T>::F ()
  : GenericDifferentiableFunction<T>
  (1, 1, "1 + x²")
{}

template <typename T>
void
F<T>::impl_compute (result_ref result, const_argument_ref x)
  const
{
  result[0] = 1. + x[0] * x[0];
}

template <>
void
F<EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type)
  const
{
  grad.insert (0) = 2. * x[0];
}

template <typename T>
void
F<T>::impl_gradient (gradient_ref grad, const_argument_ref x, size_type)
  const
{
  grad[0] = 2. * x[0];
}

BOOST_AUTO_TEST_CASE_TEMPLATE (nondifferentiable_derivative_test, T, functionTypes_t)
{
  boost::shared_ptr<F<T> > f = boost::make_shared<F<T> > ();
  boost::shared_ptr<GenericFunction<T> > df = derivative (f, 0);

  typename F<T>::argument_t x (1);
  x[0] = 1.;

  std::cout
    << "x: " << x << std::endl
    << "Input: " << (*f) << std::endl
    << "Output: " << (*df) << std::endl
    << "Evaluate:" << std::endl
    << (*df) (x) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END ()
