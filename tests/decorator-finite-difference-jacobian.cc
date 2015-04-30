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
#include <roboptim/core/decorator/finite-difference-gradient.hh>
#include <roboptim/core/indent.hh>

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-function.hh>

using namespace roboptim;
using namespace roboptim::visualization;
using namespace roboptim::visualization::gnuplot;

// Define a function with a correct Jacobian.
template <typename T>
struct FGood : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  FGood () : GenericDifferentiableFunction<T>
             (2, 2, "x * x + x * y + 2 * y, 3 * x * x * y")
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) =   argument[0] * argument[0]
      + argument[0] * argument[1]
      +          2. * argument[1];
    result (1) = 3. * argument[0] * argument[0] * argument[1];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref argument,
		      size_type) const;

  void impl_jacobian (jacobian_ref jacobian,
                      const_argument_ref argument) const;

};


template <typename T>
void
FGood<T>::impl_gradient
(gradient_ref, const_argument_ref, size_type) const
{
  // This method should not be called since we provide impl_jacobian
  assert(0);
}


template <>
void
FGood<EigenMatrixSparse>::impl_jacobian
(jacobian_ref jacobian, const_argument_ref argument) const
{
  jacobian.setZero ();
  jacobian.insert(0,0) = 2. * argument[0] + argument[1];
  jacobian.insert(0,1) =      argument[0] + 2.;
  jacobian.insert(1,0) = 6. * argument[0] * argument[1];
  jacobian.insert(1,1) = 3. * argument[0] * argument[0];
}

template <typename T>
void
FGood<T>::impl_jacobian
(jacobian_ref jacobian, const_argument_ref argument) const
{
  jacobian.setZero ();
  jacobian(0,0) = 2. * argument[0] + argument[1];
  jacobian(0,1) =      argument[0] + 2.;
  jacobian(1,0) = 6. * argument[0] * argument[1];
  jacobian(1,1) = 3. * argument[0] * argument[0];
}


// Define a function with a bad Jacobian.
template <typename T>
struct FBad : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  FBad () : GenericDifferentiableFunction<T>
            (2, 2, "x * x + x * y + 2 * y, 3 * x * x * y")
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) =   argument[0] * argument[0]
      + argument[0] * argument[1]
      +          2. * argument[1];
    result (1) = 3. * argument[0] * argument[0] * argument[1];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref argument,
		      size_type) const;

  void impl_jacobian (jacobian_ref jacobian,
                      const_argument_ref argument) const;
};

template <typename T>
void
FBad<T>::impl_gradient
(gradient_ref, const_argument_ref, size_type) const
{
  // This method should not be called since we provide impl_jacobian
  assert(0);
}


template <>
void
FBad<EigenMatrixSparse>::impl_jacobian
(jacobian_ref jacobian, const_argument_ref argument) const
{
  jacobian.setZero ();
  jacobian.insert(0,0) = 2. * argument[0] + argument[1];
  jacobian.insert(0,1) =      argument[0] + 42.;
  jacobian.insert(1,0) = 6. * argument[0] * argument[1];
  jacobian.insert(1,1) = 3. * argument[0] * argument[0];
}

template <typename T>
void
FBad<T>::impl_jacobian
(jacobian_ref jacobian, const_argument_ref argument) const
{
  jacobian.setZero ();
  jacobian(0,0) = 2. * argument[0] + argument[1];
  jacobian(0,1) =      argument[0] + 42.;
  jacobian(1,0) = 6. * argument[0] * argument[1];
  jacobian(1,1) = 3. * argument[0] * argument[0];
}


template <typename T>
void displayJacobian
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<T>&,
 typename GenericDifferentiableFunction<T>::const_argument_ref);

template <typename T>
void displayJacobian
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<T>& function,
 typename GenericDifferentiableFunction<T>::const_argument_ref x)
{
  GenericFiniteDifferenceGradient<T> fdfunction (function);
  typename GenericFiniteDifferenceGradient<T>::jacobian_t jac =
    function.jacobian (x);
  typename GenericFiniteDifferenceGradient<T>::jacobian_t fdjac =
    fdfunction.jacobian (x);

  (*output) << "#" << jac << std::endl
            << "#" << fdjac << std::endl;
}


template <>
void displayJacobian<roboptim::EigenMatrixSparse>
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<roboptim::EigenMatrixSparse>& function,
 GenericDifferentiableFunction<roboptim::EigenMatrixSparse>::
 const_argument_ref x)
{
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>
    fdfunction (function);
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>::
    jacobian_t jac = function.jacobian (x);
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>::
    jacobian_t fdjac = fdfunction.jacobian (x);

  (*output) << "#" << sparse_to_dense(jac) << std::endl
	    << "#" << sparse_to_dense(fdjac) << std::endl;
}

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_AUTO_TEST_CASE_TEMPLATE (finite_difference_jacobian, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("decorator-finite-difference-jacobian");

  FGood<T> fg;
  FBad<T> fb;

  typename FGood<T>::vector_t x (2);

  for (double i = -10.; i < 10.; i += 1.)
    {
      x[0] = i;
      x[1] = 2. * i + 0.5;

      (*output) << "#Check Jacobian at x = " << x << std::endl;

      (*output) << "# Good" << std::endl;
      displayJacobian (output, fg, x);

      (*output) << "# Bad" << std::endl;
      displayJacobian (output, fb, x);

      BOOST_CHECK (checkJacobian (fg, x));
      BOOST_CHECK (! checkJacobian (fb, x));
    }

  std::cout << output->str () << std::endl;

  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
