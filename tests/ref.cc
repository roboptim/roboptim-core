// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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
#include <roboptim/core/differentiable-function.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

struct F : public DifferentiableFunction
{
  F () : DifferentiableFunction (4, 2,
                                 "x₀ * x₃ * (x₀ + x₁ + x₂) + x₃, "
                                 "x₀²")
  {
  }

  void
  impl_compute (result_ref result, const_argument_ref x) const
  {
    result (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
    result (1) = x[0] * x[0];
  }

  void
  impl_gradient (gradient_ref grad, const_argument_ref x,
                 size_type functionId) const
  {
    switch (functionId)
    {
      case 0:
        {
          grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
          grad[1] = x[0] * x[3];
          grad[2] = x[0] * x[3] + 1;
          grad[3] = x[0] * (x[0] + x[1] + x[2]);
        }
        break;

      case 1:
        {
          grad[0] = 2. * x[0];
        }
        break;

      default:
        abort ();
    }
  }
};


BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (ref)
{
  output = retrievePattern ("ref");

  // Instantiate the function and the problem.
  F f;

  F::value_type x[8] = {0., 1., 0., 1., 0., 1., 0., 1. };
  F::jacobian_t jac (2,4);
  jac.setZero ();

  Eigen::Map<const Function::vector_t, Eigen::RowMajor, Eigen::Stride<1, 2> >
    map_x (x, 4, Eigen::Stride<1,2> ());

  F::jacobian_ref jac_ref (jac);
  F::const_argument_ref x_ref (map_x);

  Eigen::internal::set_is_malloc_allowed (false);
  f.jacobian (jac_ref, x_ref);
  Eigen::internal::set_is_malloc_allowed (true);

  (*output) << map_x << std::endl;
  (*output) << f << std::endl;
  (*output) << f (map_x) << std::endl;
  (*output) << f.jacobian (map_x) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
