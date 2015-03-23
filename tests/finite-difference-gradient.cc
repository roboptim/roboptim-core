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

// Define a function with a correct gradient.
template <typename T>
struct FGood : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  FGood () : GenericDifferentiableFunction<T> (1, 1, "x * x")
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = argument[0] * argument[0];
  }

  void impl_gradient (gradient_ref gradient,
		      const_argument_ref argument, size_type) const;

};

template <>
void
FGood<EigenMatrixSparse>::impl_gradient
(gradient_ref gradient, const_argument_ref argument, size_type) const
{
  gradient.insert (0) = 2 * argument[0];
}

template <typename T>
void
FGood<T>::impl_gradient (gradient_ref gradient,
			 const_argument_ref argument, size_type) const
{
  gradient (0) = 2 * argument[0];
}


// Define a function with a bad gradient.
template <typename T>
struct FBad : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  FBad () : GenericDifferentiableFunction<T> (1, 1, "x * x")
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = argument[0] * argument[0];
  }

  void impl_gradient (gradient_ref gradient,
		      const_argument_ref argument, size_type) const;
};

template <>
void
FBad<EigenMatrixSparse>::impl_gradient
(gradient_ref gradient, const_argument_ref argument, size_type) const
{
  gradient.insert (0) = 5 * argument[0] + 42;
}

template <typename T>
void
FBad<T>::impl_gradient (gradient_ref gradient,
			const_argument_ref argument, size_type) const
{
  gradient (0) = 5 * argument[0] + 42;
}


// Define a polynomial function.
template <typename T>
struct Polynomial : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  Polynomial () : GenericDifferentiableFunction<T> (1, 1)
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = -24 * argument[0] * argument[0] + 33 * argument[0] + 5;
  }

  void impl_gradient (gradient_ref gradient,
		      const_argument_ref argument, size_type) const;
};

template <>
void
Polynomial<EigenMatrixSparse>::impl_gradient
(gradient_ref gradient, const_argument_ref argument, size_type) const
{
  gradient.insert (0) = -42 * argument[0] + 33;
}

template <typename T>
void
Polynomial<T>::impl_gradient (gradient_ref gradient,
			      const_argument_ref argument, size_type)
  const
{
  gradient (0) = -42 * argument[0] + 33;
}


// Define a function that draws a circle.
template <typename T>
struct CircleXY : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  CircleXY () : GenericDifferentiableFunction<T> (1, 2)
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = sin (argument[0]);
    result (1) = cos (argument[0]);
  }

  void impl_gradient (gradient_ref result,
		      const_argument_ref argument,
		      size_type idFunction) const;
};

template <>
void
CircleXY<EigenMatrixSparse>::impl_gradient (gradient_ref gradient,
					    const_argument_ref argument,
					    size_type idFunction) const
{
  switch (idFunction)
    {
    case 0:
      gradient.insert (0) = cos (argument[0]);
      break;

    case 1:
      gradient.insert (0) = -sin (argument[0]);
      break;

    default:
      assert (0);
    }
}

template <typename T>
void
CircleXY<T>::impl_gradient (gradient_ref gradient,
			    const_argument_ref argument,
			    size_type idFunction) const
{
  switch (idFunction)
    {
    case 0:
      gradient (0) = cos (argument[0]);
      break;

    case 1:
      gradient (0) = -sin (argument[0]);
      break;

    default:
      assert (0);
    }
}


// Define ``f(x,y) = x * y'' function.
template <typename T>
struct Times : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  Times () : GenericDifferentiableFunction<T> (2, 1)
  {}

  void impl_compute (result_ref result,
		     const_argument_ref argument) const
  {
    result (0) = argument[0] * argument[1];
  }

  void impl_gradient (gradient_ref gradient,
		      const_argument_ref argument,
		      size_type) const;
};

template <>
void
Times<EigenMatrixSparse>::impl_gradient (gradient_ref gradient,
					 const_argument_ref argument,
					 size_type) const
{
  gradient.insert (0) = argument[1];
  gradient.insert (1) = argument[0];
}

template <typename T>
void
Times<T>::impl_gradient (gradient_ref gradient,
			 const_argument_ref argument,
			 size_type) const
{
  gradient (0) = argument[1];
  gradient (1) = argument[0];
}


template <typename T>
void displayGradient
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<T>&,
 typename GenericDifferentiableFunction<T>::const_argument_ref,
 typename GenericDifferentiableFunction<T>::size_type i = 0);

template <typename T>
void
displayGradient
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<T>& function,
 typename GenericDifferentiableFunction<T>::const_argument_ref x,
 typename GenericDifferentiableFunction<T>::size_type i)
{
  GenericFiniteDifferenceGradient<T> fdfunction (function);
  typename GenericFiniteDifferenceGradient<T>::gradient_t grad =
    function.gradient (x, i);
  typename GenericFiniteDifferenceGradient<T>::gradient_t fdgrad =
    fdfunction.gradient (x, i);

  (*output) << "#" << grad << std::endl
	    << "#" << fdgrad << std::endl;
}

template <>
void
displayGradient<roboptim::EigenMatrixSparse>
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 const GenericDifferentiableFunction<roboptim::EigenMatrixSparse>& function,
 GenericDifferentiableFunction<roboptim::EigenMatrixSparse>::const_argument_ref x,
 GenericDifferentiableFunction<roboptim::EigenMatrixSparse>::
 size_type i)
{
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>
    fdfunction (function);
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>::
    gradient_t grad = function.gradient (x, i);
  GenericFiniteDifferenceGradient<roboptim::EigenMatrixSparse>::
    gradient_t fdgrad = fdfunction.gradient (x, i);

  (*output) << "#" << sparse_to_dense(grad) << std::endl
	    << "#" << sparse_to_dense(fdgrad) << std::endl;
}

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_AUTO_TEST_CASE_TEMPLATE (finite_difference_gradient, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("finite-difference-gradient");

  FGood<T> fg;
  FBad<T> fb;
  CircleXY<T> sq;
  Times<T> times;

  typename FGood<T>::vector_t x (1);

  for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
    {
      (*output) << "#Check gradient at x = " << x[0] << std::endl;

      (*output) << "# Good" << std::endl;
      displayGradient (output, fg, x);

      (*output) << "# Bad" << std::endl;
      displayGradient (output, fb, x);

      (*output) << "# Circle" << std::endl;
      displayGradient (output, sq, x);
      displayGradient (output, sq, x, 1);

      BOOST_CHECK (checkGradient (fg, 0, x));
      BOOST_CHECK (! checkGradient (fb, 0, x));
      BOOST_CHECK_THROW (checkGradientAndThrow (fb, 0, x),
			 ::roboptim::BadGradient<T>);
      BOOST_CHECK_THROW (checkJacobianAndThrow (fb, x),
			 ::roboptim::BadJacobian<T>);


      BOOST_CHECK (checkGradient (sq, 0, x));
      BOOST_CHECK (checkGradient (sq, 1, x));
      checkGradientAndThrow (sq, 0, x);
      checkGradientAndThrow (sq, 1, x);
    }

  x.resize (2);
  for (x[1] = -10.; x[1] < 10.; x[1] += 1.)
    for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
      {
	(*output) << "# Times at x = " << x << std::endl;
	displayGradient (output, times, x);
	BOOST_CHECK (checkGradient (times, 0, x));
	checkGradientAndThrow (times, 0, x);
      }

  Gnuplot gnuplot = Gnuplot::make_interactive_gnuplot (false);

  GenericFiniteDifferenceGradient<
    T,
    finiteDifferenceGradientPolicies::Simple<T> > fg_fd (fg, 10.);

  Polynomial<T> p;
  GenericFiniteDifferenceGradient<
    T,
    finiteDifferenceGradientPolicies::Simple<T> >
    p_fd (p, 10.);

  Function::discreteInterval_t interval (-100., 100., 1.);

  (*output)
    << (gnuplot
	<< set ("multiplot layout 2,2")
	<< plot (fg, interval)
	<< plot (fg_fd, interval)
	<< plot (p, interval)
	<< plot (p_fd, interval)
	<< unset ("multiplot")
	);

  std::cout << output->str () << std::endl;

  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
