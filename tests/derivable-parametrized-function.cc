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
#include <roboptim/core/identity-function.hh>
#include <roboptim/core/derivable-parametrized-function.hh>

using namespace roboptim;

// Define a simple function.
struct ParametrizedDF : public DerivableParametrizedFunction<IdentityFunction>
{
  ParametrizedDF () : DerivableParametrizedFunction<IdentityFunction> (1, 1, 1)
  {}

  result_t impl_compute (const argument_t& argument) const throw ()
  {
    return result_t (argument);
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t&,
		      size_type = 0,
		      size_type order = 0) const throw ()
  {
    gradient.clear ();
    switch (order)
      {
      case 0:
	gradient[0] = 1.;
	break;
      case 1:
	gradient[0] = 0.;
	break;
      }
  }
};

#define CHECKME(PVALUE)						\
  {								\
    std::cout << "Parameter is " << PVALUE << std::endl;	\
    parameter[0] = (PVALUE);					\
    IdentityFunction cst = pf (parameter);			\
								\
    IdentityFunction::vector_t x (1);				\
    x[0] = 31.;							\
    std::cout							\
      << cst << std::endl					\
      << "Evaluate: " << std::endl				\
      << cst (x) << std::endl					\
      << "Gradient: " << std::endl				\
      << cst.gradient (x) << std::endl				\
      << "Jacobian: " << std::endl				\
      << cst.jacobian (x) << std::endl				\
      << "Derivative w.r.t params: " << std::endl		\
      << cst.jacobian (x) << std::endl				\
      << std::endl;						\
  }								\

int run_test ()
{
  ParametrizedDF pf;

  ParametrizedDF::argument_t parameter (1);
  parameter.clear ();

  CHECKME (0.);
  CHECKME (3.14);
  CHECKME (42.);

  {
    parameter[0] = 128.;
    IdentityFunction::vector_t x (1);
    x[0] = 256.;

    // Natural evaluation in one line.
    IdentityFunction::result_t res = pf (parameter) (x);

    assert (res[0] == 128. + 256.);
  }

  return 0;
}

GENERATE_TEST ()
