// Copyright (C) 2015 by Félix Darricau, AIST, CNRS, EPITA
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
#include <roboptim/core/sum-of-c1-squares.hh>

using namespace roboptim;

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

BOOST_AUTO_TEST_CASE (sum_of_c1_squares)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("sum-of-c1-squares");

  boost::shared_ptr<F> fptr =
    boost::make_shared<F>();

  SumOfC1Squares fSum(fptr, "null");

  SumOfC1Squares::vector_t x (4);
  x << 1., 2., 1., 2.;
  SumOfC1Squares::gradient_t grad (fSum.gradientSize ());

  (*output) << fSum << std::endl;

  BOOST_CHECK (fSum.inputSize () == 4);

  BOOST_CHECK (fSum.outputSize () == 1);

  BOOST_CHECK (fSum.getName () == "null");

  BOOST_CHECK (fSum.isValidResult (fSum (x)));

  BOOST_CHECK (fSum(x)[0] == 101);

  (*output) << fSum.gradient (x) << std::endl;

  fSum.gradient (grad, x);
  (*output) << grad << std::endl;

  BOOST_CHECK (fSum.gradientSize () == 4);

  BOOST_CHECK (fSum.isValidGradient (fSum.gradient (x)));


  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
