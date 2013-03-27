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

#include "shared-tests/common.hh"

#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

template <typename T>
struct Null : public GenericDifferentiableFunction<T>
{
  typedef GenericDifferentiableFunction<T> parent_t;
  typedef typename parent_t::size_type size_type;
  typedef typename parent_t::argument_t argument_t;
  typedef typename parent_t::result_t result_t;
  typedef typename parent_t::gradient_t gradient_t;

  Null () : GenericDifferentiableFunction<T> (1, 1, "null function")
  {}

  void impl_compute (result_t& res, const argument_t&) const throw ()
  {
    res.setZero ();
  }

  void impl_gradient (gradient_t& grad, const argument_t&,
		      size_type) const throw ()
  {
    grad.setZero ();
  }
};

template <typename T>
struct NoTitle : public GenericDifferentiableFunction<T>
{
  typedef GenericDifferentiableFunction<T> parent_t;
  typedef typename parent_t::size_type size_type;
  typedef typename parent_t::argument_t argument_t;
  typedef typename parent_t::result_t result_t;
  typedef typename parent_t::gradient_t gradient_t;

  NoTitle () : GenericDifferentiableFunction<T> (1, 1)
  {}

  void impl_compute (result_t& res, const argument_t&) const throw ()
  {
    res.setZero ();
  }

  void impl_gradient (gradient_t& grad, const argument_t&,
		      size_type) const throw ()
  {
    grad.setZero ();
  }
};

template <typename T>
struct getPattern
{
  const char* operator () ();
};

template <>
const char* getPattern< ::roboptim::EigenMatrixSparse>::operator () ()
{
  return "derivable-function-sparse";
}

template <typename T>
const char* getPattern<T>::operator () ()
{
  return "derivable-function";
}

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_AUTO_TEST_CASE_TEMPLATE (derivable_function, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern (getPattern<T> () ());

  Null<T> null;
  NoTitle<T> notitle;

  typename Null<T>::vector_t x (1);
  typename Null<T>::gradient_t grad (null.gradientSize ());
  x[0] = 42.;

  (*output) << null << std::endl
	    << notitle << std::endl;

  (*output) << null.inputSize () << std::endl
	    << notitle.inputSize () << std::endl;

  (*output) << null.outputSize () << std::endl
	    << notitle.outputSize () << std::endl;

  (*output) << null.getName () << std::endl
	    << notitle.getName () << std::endl;

  (*output) << null.isValidResult (null (x)) << std::endl
	    << notitle.isValidResult (notitle (x)) << std::endl;

  (*output) << null (x) << std::endl
	    << notitle (x) << std::endl;

  (*output) << null.gradient (x) << std::endl
	    << notitle.gradient (x) << std::endl;

  null.gradient (grad, x);
  (*output) << grad << std::endl;
  notitle.gradient (grad, x);
  (*output) << grad << std::endl;

  (*output) << null.gradientSize () << std::endl
	    << notitle.gradientSize () << std::endl;

  (*output) << null.jacobianSize () << std::endl
	    << notitle.jacobianSize () << std::endl;

  (*output) << null.isValidGradient (null.gradient (x)) << std::endl
	    << notitle.isValidGradient (notitle.gradient (x)) << std::endl;

  (*output) << null.isValidJacobian (null.jacobian (x)) << std::endl
	    << notitle.isValidJacobian (notitle.jacobian (x)) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
