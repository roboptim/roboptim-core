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
#include <roboptim/core/constant-function.hh>

using namespace roboptim;

int run_test ()
{
  ConstantFunction::vector_t offset (4);
  offset[0] = 12.;
  offset[1] = 46.;
  offset[2] = 2.;
  offset[3] = -9.;

  ConstantFunction cst (offset);

  ConstantFunction::vector_t x (4);
  x.clear ();

  std::cout
    << cst << std::endl
    << "Evaluate: " << std::endl
    << cst (x) << std::endl
    << "Gradient: " << std::endl
    << cst.gradient (x) << std::endl
    << "Jacobian: " << std::endl
    << cst.jacobian (x) << std::endl;

  return 0;
}

GENERATE_TEST ()
