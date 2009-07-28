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

#include <roboptim/core/io.hh>
#include <roboptim/core/numeric-quadratic-function.hh>

#include "common.hh"

using namespace roboptim;

typedef DummySolver solver_t;

int run_test ()
{
  NumericQuadraticFunction::matrix_t a (5, 5);
  NumericQuadraticFunction::vector_t b (5);
  NumericQuadraticFunction::vector_t x (5);

  a.clear ();
  b.clear ();
  x.clear ();

  a(0, 0) = 1.1, a(0, 1) = 1.2, a(0, 2) = 1.3, a(0, 3) = 1.4, a(0, 4) = 1.5;
  a(1, 0) = 1.2, a(1, 1) = 2.2, a(1, 2) = 2.3, a(1, 3) = 2.4, a(1, 4) = 2.5;
  a(2, 0) = 1.3, a(2, 1) = 2.3, a(2, 2) = 3.3, a(2, 3) = 3.4, a(2, 4) = 3.5;
  a(3, 0) = 1.4, a(3, 1) = 2.4, a(3, 2) = 3.4, a(3, 3) = 4.4, a(3, 4) = 4.5;
  a(4, 0) = 1.5, a(4, 1) = 2.5, a(4, 2) = 3.5, a(4, 3) = 4.5, a(4, 4) = 5.5;

  b[0] = 2.1;
  b[1] = 4.3;
  b[2] = 6.5;
  b[3] = 8.7;

  NumericQuadraticFunction f (a, b);

  std::cout << f << std::endl;

  x[0] = 0.1;
  x[1] = 1.2;
  x[2] = 2.3;
  x[3] = 3.4;
  x[4] = 4.5;

  std::cout << "f(x) = " << f (x) << std::endl;
  std::cout << "J(x) = " << f.jacobian (x) << std::endl;
  std::cout << "G(x) = " << f.gradient (x, 0) << std::endl;
  std::cout << "H(x) = " << f.hessian (x, 0) << std::endl;

  return 0;
}

GENERATE_TEST ()
