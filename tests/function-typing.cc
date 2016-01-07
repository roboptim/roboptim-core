// Copyright (C) 2015 by Félix Darricau, AIST, CNRS, EPITA
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
#include <roboptim/core/function.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/twice-differentiable-function.hh>
#include <roboptim/core/linear-function.hh>
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/quadratic-function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/polynomial.hh>
#include <roboptim/core/n-times-derivable-function.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

template <typename T>
struct F : GenericDifferentiableFunction<T>
{
  F ()
    : GenericDifferentiableFunction<T>(1,1, "differentiable")
  {}

  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_(GenericDifferentiableFunction<T>);
  virtual void impl_compute(result_ref, const_argument_ref) const
  {}
  virtual void impl_gradient(gradient_ref, const_argument_ref, size_type) const
  {}
  virtual void impl_jacobian(jacobian_ref, const_argument_ref) const
  {}
};

template <typename T>
struct G : GenericTwiceDifferentiableFunction<T>
{
  G ()
    : GenericTwiceDifferentiableFunction<T>(1,1, "twice diff")
  {}

  ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_(GenericTwiceDifferentiableFunction<T>);
  virtual void impl_compute(result_ref, const_argument_ref) const
  {}
  virtual void impl_gradient(gradient_ref, const_argument_ref, size_type) const
  {}
  virtual void impl_jacobian(jacobian_ref, const_argument_ref) const
  {}
  virtual void impl_hessian(hessian_ref, const_argument_ref, size_type) const
  {}
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (differentiable_function_typing, T, functionTypes_t)
{
  F<T> func;

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));

  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == false);
  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == false);
  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == false);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == false);
  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == false);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == false);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == false);

  BOOST_CHECK_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericConstantFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (twice_differentiable_function_typing, T, functionTypes_t)
{
  G<T> func;

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);
  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == false);
  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == false);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == false);
  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == false);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == false);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == false);

  BOOST_CHECK_THROW(func.template castInto<GenericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericConstantFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (numeric_quadratic_function_typing, T, functionTypes_t)
{
  typename GenericNumericQuadraticFunction<T>::vector_t B (1);
  B[0] = 1;
  typename GenericNumericLinearFunction<T>::matrix_t A (1,1);
  A.setZero();
  GenericNumericQuadraticFunction<T> func(A,B);

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);
  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == true);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == true);
  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true));

  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == false);
  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == false);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == false);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == false);

  BOOST_CHECK_THROW(func.template castInto<GenericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericConstantFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (numeric_linear_function_typing, T, functionTypes_t)
{
  typename GenericNumericLinearFunction<T>::vector_t B (1);
  B[0] = 1;
  typename GenericNumericLinearFunction<T>::matrix_t A (1,1);
  A.setZero();
  GenericNumericLinearFunction<T> func(A,B);

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);
  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == true);
  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == true);
  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == true);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true));

  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == false);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == false);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == false);

  BOOST_CHECK_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericConstantFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (constant_function_typing, T, functionTypes_t)
{
  typename GenericConstantFunction<T>::vector_t offset (1);
  offset[0] = 1;
  GenericConstantFunction<T> func(offset);

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);
  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == true);
  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == true);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == true);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (true));

  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == false);
  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == false);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == false);

  BOOST_CHECK_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (polynomial_function_typing, T, functionTypes_t)
{
  typename Polynomial<T>::vector_t offset (1);
  offset[0] = 1;
  Polynomial<T> func(offset);

  BOOST_CHECK(func.template asType<GenericFunction<T> >()                     == true);
  BOOST_CHECK(func.template asType<GenericDifferentiableFunction<T> >()       == true);
  BOOST_CHECK(func.template asType<GenericTwiceDifferentiableFunction<T> >()  == true);
  BOOST_CHECK(func.template asType<Polynomial<T> >()                          == true);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericTwiceDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func.template castInto<Polynomial<T> > (true));

  BOOST_CHECK(func.template asType<GenericLinearFunction<T> >()               == false);
  BOOST_CHECK(func.template asType<GenericConstantFunction<T> >()             == false);
  BOOST_CHECK(func.template asType<GenericNumericLinearFunction<T> >()        == false);
  BOOST_CHECK(func.template asType<GenericQuadraticFunction<T> >()            == false);
  BOOST_CHECK(func.template asType<GenericNumericQuadraticFunction<T> >()     == false);

  BOOST_CHECK_THROW(func.template castInto<GenericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericLinearFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(func.template castInto<GenericConstantFunction<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func.template castInto<GenericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericLinearFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericNumericQuadraticFunction<T> > (false));
  BOOST_CHECK_NO_THROW(func.template castInto<GenericConstantFunction<T> > (false));
}

BOOST_AUTO_TEST_CASE_TEMPLATE (shared_ptr_cast, T, functionTypes_t)
{
  boost::shared_ptr<G<T> > func (new G<T> ());
  boost::shared_ptr<GenericFunction<T> > f = castInto<G<T>, GenericFunction<T> > (func);
  boost::shared_ptr<GenericDifferentiableFunction<T> > df = castInto<G<T>, GenericDifferentiableFunction<T> > (func);

  // Check ref counters
  BOOST_CHECK(func.use_count () == 3);
  BOOST_CHECK(f.use_count () == 3);
  BOOST_CHECK(df.use_count () == 3);

  // Check that they all have the same properties
  BOOST_CHECK(func->template asType<GenericFunction<T> >());
  BOOST_CHECK(func->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(func->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK(f->template asType<GenericFunction<T> >());
  BOOST_CHECK(f->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(f->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK(df->template asType<GenericFunction<T> >());
  BOOST_CHECK(df->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(df->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK_NO_THROW(func->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_NO_THROW(f->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(f->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(f->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_NO_THROW(df->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(df->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(df->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_THROW(func->template castInto<Polynomial<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(f->template castInto<Polynomial<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(df->template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func->template castInto<Polynomial<T> > (false));
  BOOST_CHECK_NO_THROW(f->template castInto<Polynomial<T> > (false));
  BOOST_CHECK_NO_THROW(df->template castInto<Polynomial<T> > (false));
}

// TODO: avoid code duplicationc
BOOST_AUTO_TEST_CASE_TEMPLATE (const_shared_ptr_cast, T, functionTypes_t)
{
  const boost::shared_ptr<G<T> > func (new G<T> ());
  const boost::shared_ptr<GenericFunction<T> > f = castInto<G<T>, GenericFunction<T> > (func);
  const boost::shared_ptr<GenericDifferentiableFunction<T> > df = castInto<G<T>, GenericDifferentiableFunction<T> > (func);

  // Check ref counters
  BOOST_CHECK(func.use_count () == 3);
  BOOST_CHECK(f.use_count () == 3);
  BOOST_CHECK(df.use_count () == 3);

  // Check that they all have the same properties
  BOOST_CHECK(func->template asType<GenericFunction<T> >());
  BOOST_CHECK(func->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(func->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK(f->template asType<GenericFunction<T> >());
  BOOST_CHECK(f->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(f->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK(df->template asType<GenericFunction<T> >());
  BOOST_CHECK(df->template asType<GenericDifferentiableFunction<T> >());
  BOOST_CHECK(df->template asType<GenericTwiceDifferentiableFunction<T> >());

  BOOST_CHECK_NO_THROW(func->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(func->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_NO_THROW(f->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(f->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(f->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_NO_THROW(df->template castInto<GenericFunction<T> > (true));
  BOOST_CHECK_NO_THROW(df->template castInto<GenericDifferentiableFunction<T> > (true));
  BOOST_CHECK_NO_THROW(df->template castInto<GenericTwiceDifferentiableFunction<T> > (true));

  BOOST_CHECK_THROW(func->template castInto<Polynomial<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(f->template castInto<Polynomial<T> > (true), std::runtime_error);
  BOOST_CHECK_THROW(df->template castInto<Polynomial<T> > (true), std::runtime_error);

  BOOST_CHECK_NO_THROW(func->template castInto<Polynomial<T> > (false));
  BOOST_CHECK_NO_THROW(f->template castInto<Polynomial<T> > (false));
  BOOST_CHECK_NO_THROW(df->template castInto<Polynomial<T> > (false));
}

BOOST_AUTO_TEST_SUITE_END ()
