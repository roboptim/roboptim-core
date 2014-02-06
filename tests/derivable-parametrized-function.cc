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

#include "shared-tests/fixture.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/function/identity.hh>
#include <roboptim/core/derivable-parametrized-function.hh>

using namespace roboptim;

// Define a simple function.
struct ParametrizedDF : public DerivableParametrizedFunction<IdentityFunction>
{
  ParametrizedDF () : DerivableParametrizedFunction<IdentityFunction> (1, 1, 1)
  {}

  result_t impl_compute (const argument_t& argument) const
  {
    return result_t (argument);
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t&,
		      size_type = 0,
		      size_type order = 0) const
  {
    gradient.setZero ();
    switch (order)
      {
      case 0:
	gradient[0] = 1.;
	break;
      default:
	gradient[0] = 0.;
	break;
      }
  }
};

#define CHECKME(PVALUE)							\
  {									\
    (*output) << "Parameter is " << PVALUE << std::endl;		\
    parameter[0] = (PVALUE);						\
    IdentityFunction cst = pf (parameter);				\
									\
    ParametrizedDF::vector_t x (1);					\
    x[0] = 31.;								\
    (*output)								\
      << cst << std::endl						\
      << "Evaluate:" << std::endl					\
      << cst (x) << std::endl						\
      << "Gradient:" << std::endl					\
      << cst.gradient (x) << std::endl					\
      << "Jacobian:" << std::endl					\
      << cst.jacobian (x) << std::endl					\
      << "Derivative w.r.t params:" << std::endl			\
      << cst.jacobian (x) << std::endl					\
      << std::endl;							\
  }

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (derivable_parametrized_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("derivable-parametrized-function");

  ParametrizedDF pf;

  ParametrizedDF::vector_t parameter (1);
  parameter.setZero ();

  CHECKME (0.);
  CHECKME (3.14);
  CHECKME (42.);

  {
    parameter[0] = 128.;
    IdentityFunction::vector_t x (1);
    x[0] = 256.;

    // Natural evaluation in one line.
    IdentityFunction::result_t res = pf (parameter) (x);

    (*output) << pf << std::endl
      << "Evaluate:" << std::endl
      << pf (x) << std::endl
      << "Gradient (order = 0):" << std::endl
      << pf.gradient (x, 0) << std::endl
      << "Gradient (order = 1):" << std::endl
      << pf.gradient (x, 0, 1) << std::endl
      << "Jacobian (order = 0):" << std::endl
      << pf.jacobian (x) << std::endl
      << "Jacobian (order = 1):" << std::endl
      << pf.jacobian (x, 1) << std::endl;

    BOOST_CHECK_EQUAL (res[0], 128. + 256.);
    BOOST_CHECK (pf.functionInputSize () == 1);
    BOOST_CHECK (pf.functionOutputSize () == 1);
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
