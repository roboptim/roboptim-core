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

  std::cout << null << std::endl
	    << notitle << std::endl;

  return 0;
}

GENERATE_TEST ()
