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
#include <roboptim/core/constant-function.hh>
#include <roboptim/core/parametrized-function.hh>

using namespace roboptim;

// Define a simple function.
struct ParametrizedF : public ParametrizedFunction<ConstantFunction>
{
  ParametrizedF () : ParametrizedFunction<ConstantFunction> (1, 1, 1)
  {}

  result_t impl_compute (const argument_t& argument) const throw ()
  {
    return result_t (argument);
  }
};

#define CHECKME(PVALUE)						\
  {								\
    std::cout << "Parameter is " << PVALUE << std::endl;	\
    parameter[0] = (PVALUE);					\
    ConstantFunction cst = pf (parameter);			\
								\
    ConstantFunction::vector_t x (1);				\
    x[0] = 31.;							\
    std::cout							\
      << cst << std::endl					\
      << "Evaluate: " << std::endl				\
      << cst (x) << std::endl					\
      << "Gradient: " << std::endl				\
      << cst.gradient (x) << std::endl				\
      << "Jacobian: " << std::endl				\
      << cst.jacobian (x) << std::endl				\
      << std::endl;						\
  }								\

int run_test ()
{
  ParametrizedF pf;

  ParametrizedF::argument_t parameter (1);
  parameter.clear ();

  CHECKME (0.);
  CHECKME (3.14);
  CHECKME (42.);

  {
    parameter[0] = 128.;
    ConstantFunction::vector_t x (1);
    x[0] = 256.;

    // Natural evaluation in one line.
    ConstantFunction::result_t res = pf (parameter) (x);

    assert (res[0] == 128.);
  }

  return 0;
}

GENERATE_TEST ()
