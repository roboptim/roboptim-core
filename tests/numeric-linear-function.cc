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

#include <roboptim/core/numeric-linear-function.hh>
#include "common.hh"

using namespace roboptim;

typedef DummySolver solver_t;

int run_test ()
{
  NumericLinearFunction::matrix_t a (1, 5);
  NumericLinearFunction::vector_t b (1);
  NumericLinearFunction::vector_t x (5);

  a.clear ();
  b.clear ();
  x.clear ();

  a(0, 0) = 1.2;
  a(0, 1) = 3.4;
  a(0, 2) = 5.6;
  a(0, 3) = 7.8;

  b[0] = 1.;

  NumericLinearFunction f (a, b);

  std::cout << f << std::endl;

  x[0] = 0.1;
  x[1] = 1.2;
  x[2] = 2.3;
  x[3] = 3.4;
  x[4] = 4.5;

  std::cout << "f(x) = " << f (x) << std::endl;
  std::cout << "J(x) = " << f.jacobian (x) << std::endl;

  return 0;
}


GENERATE_TEST ()
