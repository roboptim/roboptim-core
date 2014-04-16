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

#include <roboptim/core/io.hh>
#include <roboptim/core/debug.hh>
#include <roboptim/core/n-times-derivable-function.hh>

using namespace roboptim;

// Define a 2-times derivable null function.
struct F2 : public NTimesDerivableFunction<2>
{
  using NTimesDerivableFunction<2>::impl_compute;

  F2 () : NTimesDerivableFunction<2> (4, "0")
  {}

  virtual void impl_compute (result_ref result, double) const
  {
    result.setZero ();
  }

  virtual void impl_derivative (gradient_ref derivative,
				double,
				size_type ROBOPTIM_DEBUG_ONLY (order = 1)) const
  {
    assert (order <= derivabilityOrder);
    derivative.setZero ();
  }
};

// Define a 10-times derivable null function.
struct F10 : public NTimesDerivableFunction<10>
{
  using NTimesDerivableFunction<10>::impl_compute;

  F10 () : NTimesDerivableFunction<10> (4, "0")
  {}

  virtual void impl_compute (result_ref result, double) const
  {
    result.setZero ();
  }

  virtual void impl_derivative (gradient_ref derivative,
				double,
				size_type ROBOPTIM_DEBUG_ONLY (order = 1)) const
  {
    assert (order <= derivabilityOrder);
    derivative.setZero ();
  }
};

#define PRINT_FUNC(func)                          \
  (*output) << (*func) << std::endl               \
            << "Evaluate:" << std::endl           \
            << (*func) (x[0]) << std::endl        \
            << "Gradient:" << std::endl           \
            << func->gradient (x, 0) << std::endl \
            << "Jacobian:" << std::endl           \
            << func->jacobian (x) << std::endl    \
            << "Max derivability order: "         \
            << func->derivabilityOrderMax () << std::endl

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (n_times_derivable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("n-times-derivable-function");

  boost::shared_ptr<F2> f2 = boost::make_shared<F2> ();
  boost::shared_ptr<F10> f10 = boost::make_shared<F10> ();

  F2::argument_t x (1);
  x.setZero ();

  PRINT_FUNC (f2);
  (*output) << std::endl;
  PRINT_FUNC (f10);

  BOOST_CHECK (f2->derivabilityOrderMax () == 2);
  BOOST_CHECK (f10->derivabilityOrderMax () == 10);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
