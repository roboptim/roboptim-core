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

#include <roboptim/core/io.hh>
#include <roboptim/core/result.hh>
#include <roboptim/core/result-with-warnings.hh>

using namespace roboptim;

int run_test ()
{
  // Instantiate some results.
  Result result (3, 8);

  assert (result.x.size () == 3);
  assert (result.value.size () == 8);
  assert (result.lambda.size () == 0);

  Result result2 (result);
  Result result3 = result;

  std::cout << result << std::endl;
  return 0;
}

GENERATE_TEST ()
