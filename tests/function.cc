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

#include <boost/mpl/list.hpp>

#include "shared-tests/fixture.hh"

#include <boost/test/test_case_template.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/function.hh>

using namespace roboptim;

template <typename T>
struct Null : public GenericFunction<T>
{
  typedef typename GenericFunction<T>::argument_t argument_t;
  typedef typename GenericFunction<T>::result_t result_t;

  Null () : GenericFunction<T> (1, 1, "null function")
  {}

  void impl_compute (result_t& res, const argument_t&) const throw ()
  {
    res.setZero ();
  }
};

template <typename T>
struct NoTitle : public GenericFunction<T>
{
  typedef typename GenericFunction<T>::argument_t argument_t;
  typedef typename GenericFunction<T>::result_t result_t;

  NoTitle () : GenericFunction<T> (1, 1)
  {}

  void impl_compute (result_t& res, const argument_t&) const throw ()
  {
    res.setZero ();
  }
};

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (null_function, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("function");

  Null<T> null;
  NoTitle<T> notitle;

  typename Null<T>::vector_t x (1);
  x[0] = 42.;

  typename Null<T>::argument_t res (null.outputSize ());

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

  null (res, x);
  (*output) << res << std::endl;

  notitle (res, x);
  (*output) << res << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}


struct F : public Function
{
  ROBOPTIM_FUNCTION_FWD_TYPEDEFS (Function);
    
  F () : Function (1, 1, "first line\nsecond line\nthirdline")
  {}
    
  void impl_compute (result_t& res, const argument_t&) const throw ()
  {
    res.setZero ();
  }
};

BOOST_AUTO_TEST_CASE(name_indentation_function)
{
  F f;
  std::cout << "Function: " << incindent << iendl << f << iendl;

  //FIXME
}

BOOST_AUTO_TEST_SUITE_END ()
