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

#include <boost/assign/list_of.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "common.hh"
#include <roboptim/core/freeze.hh>

using namespace roboptim;

int run_test ()
{
  using namespace boost::assign;

  // Freeze arguments 1, 2 and 3 (4 arguments in total).
  Freeze f (4, list_of <Freeze::frozenArgument_t> (0, 3.) (1, 7.) (2, 0.));

  // Create an argument vector.
  Freeze::vector_t v (4);
  v.clear ();

  // Evaluate at frozen point: the result is minimal (i.e. -1).
  v[0] = 3., v[1] = 7., v[2] = 0.;
  std::cout
    << "Value: " << v << std::endl
    << "Result: " << f (v) << std::endl;


  // Evaluate at another point.
  v[0] = 8., v[1] = 2., v[2] = 5., v[3] = 666.;
  std::cout
    << "Value: " << v << std::endl
    << "Result: " << f (v) << std::endl;

  return 0;
}

GENERATE_TEST ()
