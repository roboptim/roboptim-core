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

#include <cmath>
#include <boost/mpl/list.hpp>

#include "shared-tests/fixture.hh"

#include <boost/test/test_case_template.hpp>
#include <boost/format.hpp>

#include <iostream>

#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/io.hh>
#include <roboptim/core/filter/chain.hh>
#include <roboptim/core/filter/selection.hh>
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/numeric-quadratic-function.hh>

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

  typename GenericIdentityFunction<T>::vector_t x (5);
  x.setZero ();
  std::cout
    << (*fct) (x) << "\n"
    << fct->gradient (x, 0) << "\n"
    << fct->jacobian (x) << std::endl;

  BOOST_CHECK_THROW
    (boost::shared_ptr<GenericLinearFunction<T> >
     fct2 = chain (constant, selec_constant),
     std::runtime_error);
}

#define CHECK_GRADIENT(F, I, X)				\
  BOOST_CHECK_NO_THROW					\
  (							\
   try							\
     {							\
       checkGradientAndThrow ((F), (I), (X));		\
       std::cout << "-> gradient ok\n";			\
     }							\
   catch(const BadGradient<T>& e)			\
     {							\
       std::cerr << #F << ":\n" << e << std::endl;	\
       throw;						\
     } )

#define CHECK_JACOBIAN(F, X)				\
  BOOST_CHECK_NO_THROW					\
  (							\
   try							\
     {							\
       checkJacobianAndThrow ((F), (X));		\
       std::cout << "-> jacobian ok\n";			\
     }							\
   catch(const BadJacobian<T>& e)			\
     {							\
       std::cerr << #F << ":\n" << e << std::endl;	\
       throw;						\
     } )

BOOST_AUTO_TEST_CASE_TEMPLATE (chain_jacobian_test, T, functionTypes_t)
{
  typedef typename GenericNumericLinearFunction<T>::matrix_t matrix_t;
  typedef typename GenericNumericLinearFunction<T>::vector_t vector_t;
  typedef boost::shared_ptr<GenericNumericLinearFunction<T> >
    linearFunctionShPtr_t;
  typedef boost::shared_ptr<GenericNumericQuadraticFunction<T> >
    quadraticFunctionShPtr_t;
  typedef boost::shared_ptr<GenericDifferentiableFunction<T> >
    differentiableFunctionShPtr_t;

  vector_t x (2);

  vector_t Af (2);
  Af[0] = 4.;
  Af[1] = 7.;

  vector_t Bf (2);
  Bf.setZero ();

  linearFunctionShPtr_t f =
    boost::make_shared<GenericNumericLinearFunction<T> > (Af, Bf);


  // g(x) = (Z - X)^2
  vector_t Z (2);
  Z[0] = 3;
  Z[1] = 7;

  matrix_t Ag (2, 2);
  Ag.setIdentity ();
  vector_t Bg (2);
  Bg = -2 * Z;
  vector_t c (1);
  c = Z.adjoint () * Z;

  quadraticFunctionShPtr_t g =
    boost::make_shared<GenericNumericQuadraticFunction<T> > (Ag, Bg, c);

  std::cout
    << "g(x)\n" << (*g) << "\n"
    << "g(Z)\n" << (*g) (Z) * 2 << "\n";

  differentiableFunctionShPtr_t h =
    chain<
      GenericDifferentiableFunction<T>,
      GenericDifferentiableFunction<T> >
  (f, g);


  for (int i = 0; i < 100; i++)
    {
      x.setRandom ();
      x[0] = std::ceil (x[0]);
      x[1] = std::ceil (x[1]);

      Function::result_t gx = (*g)(x);
      Function::result_t Jf_gx = f->jacobian (gx);
      std::cout
	<< "jacobian f\n" << Jf_gx << "\n"
	<< "jacobian g\n" << g->jacobian (x) << "\n"
	<< "h (manual) \n";
      std::cout
	<< Jf_gx * g->jacobian (x);
      std::cout
	<< "\n"
	<< "jacobian h\n" << h->jacobian (x) << "\n";

      CHECK_GRADIENT (*g, 0, x);

      CHECK_GRADIENT (*h, 0, x);
      CHECK_GRADIENT (*h, 1, x);

      CHECK_JACOBIAN (*g, x);

      CHECK_JACOBIAN (*h, x);
      CHECK_JACOBIAN (*h, x);
    }
}

BOOST_AUTO_TEST_SUITE_END ()
