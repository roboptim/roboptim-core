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

#include <iostream>

#include <boost/numeric/ublas/io.hpp>

#include "common.hh"
#include <roboptim/core/function.hh>

using namespace roboptim;

struct Null : public Function
{
  Null () : Function (1, 1, "null function")
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    res.clear ();
  }
};

struct NoTitle : public Function
{
  NoTitle () : Function (1, 1)
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    res.clear ();
  }
};

int run_test ()
{
  Null null;
  NoTitle notitle;

  Null::vector_t x (1);
  x[0] = 42.;

  Null::argument_t res (null.outputSize ());

  std::cout << null << std::endl
	    << notitle << std::endl;

  std::cout << null.inputSize () << std::endl
	    << notitle.inputSize () << std::endl;

  std::cout << null.outputSize () << std::endl
	    << notitle.outputSize () << std::endl;

  std::cout << null.getName () << std::endl
	    << notitle.getName () << std::endl;

  std::cout << null.isValidResult (null (x)) << std::endl
	    << notitle.isValidResult (notitle (x)) << std::endl;

  std::cout << null (x) << std::endl
	    << notitle (x) << std::endl;

  null (res, x);
  std::cout << res << std::endl;

  notitle (res, x);
  std::cout << res << std::endl;

  return 0;
}

GENERATE_TEST ()
