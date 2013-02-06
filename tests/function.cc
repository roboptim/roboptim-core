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

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/function.hh>

using namespace roboptim;

struct Null : public Function
{
  Null () : Function (1, 1, "null function")
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }
};

struct NoTitle : public Function
{
  NoTitle () : Function (1, 1)
  {}

  void impl_compute (result_t res, argument_t) const throw ()
  {
    res.block (0, 0, res.rows (), res.cols ()).setZero ();
  }
};

BOOST_AUTO_TEST_CASE (null_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("function");

  Null null;
  NoTitle notitle;

  Null::vector_t x (1);
  x[0] = 42.;

  Null::vector_t res (null.outputSize ());

  (*output) << null << std::endl
	    << notitle << std::endl;

  (*output) << null.inputSize () << std::endl
	    << notitle.inputSize () << std::endl;

  (*output) << null.outputSize () << std::endl
	    << notitle.outputSize () << std::endl;

  (*output) << null.getName () << std::endl
	    << notitle.getName () << std::endl;

  Null::vector_t r = null (x);
  Null::vector_t r2 = notitle (x);
  (*output) << null.isValidResult (r) << std::endl
	    << notitle.isValidResult (r2) << std::endl;

  (*output) << null (x) << std::endl
	    << notitle (x) << std::endl;

  null (res, x);
  (*output) << res << std::endl;

  notitle (res, x);
  (*output) << res << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
