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
#include <boost/variant/get.hpp>

#include "common.hh"
#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/indent.hh>

using namespace roboptim;

struct FGood : public DerivableFunction
{
  FGood () : DerivableFunction (1, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[0];
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int) const throw ()
  {
    vector_t res (n);
    res (0) = 2 * x[0];
    return res;
  }
};


struct FBad : public DerivableFunction
{
  FBad () : DerivableFunction (1, 1)
  {}

  virtual vector_t operator () (const vector_t& x) const throw ()
  {
    vector_t res (m);
    res (0) = x[0] * x[0];
    return res;
  }

  virtual gradient_t gradient (const vector_t& x, int) const throw ()
  {
    vector_t res (n);
    res (0) = 5 * x[0] + 42;
    return res;
  }
};

void displayGradient (const DerivableFunction&, const Function::vector_t&);

void
displayGradient (const DerivableFunction& function, const Function::vector_t& x)
{
  FiniteDifferenceGradient fdfunction (function);
  DerivableFunction::gradient_t grad = function.gradient (x, 0);
  DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, 0);

  std::cout << grad << std::endl << fdgrad;
}

int run_test ()
{
  FGood fg;
  FBad fb;

  Function::vector_t x (1);

  for (x[0] = -10.; x[0] < 10.; x[0] += 1.)
    {
      std::cout << "Check gradient at x = " << x[0] << incindent << std::endl;

      std::cout << "Good" << incindent << std::endl;
      displayGradient (fg, x);
      std::cout << decindent << std::endl;

      std::cout << "Bad" << incindent << std::endl;
      displayGradient (fb, x);
      std::cout << decindent << std::endl;

      assert (checkGradient (fg, 0, x));
      assert (! checkGradient (fb, 0, x));

      std::cout << decindent << std::endl;
    }
  return 0;
}

GENERATE_TEST ()
