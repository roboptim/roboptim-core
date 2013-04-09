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
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/util.hh>
#include <roboptim/core/filter/cached-function.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

struct F : public DifferentiableFunction
{
  F () : DifferentiableFunction (2, 1, "2 * x * x + y")
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    (*output) << "computation (not cached)" << std::endl;
    res.setZero ();
    res[0] = 2. * argument[0] * argument[0] + argument[1];
  }

  void impl_gradient (gradient_t& grad, const argument_t& argument,
		      size_type) const throw ()
  {
    (*output) << "gradient computation (not cached)" << std::endl;
    grad.setZero ();
    grad[0] = 4. * argument[0];
    grad[1] = 1.;
  }

  void impl_jacobian (jacobian_t& jacobian, const argument_t& argument)
    const throw ()
  {
    (*output) << "jacobian computation (not cached)" << std::endl;
    jacobian.setZero ();
    jacobian(0,0) = 4. * argument[0];
    jacobian(0,1) = 1.;
  }
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (cached_function)
{
  output = retrievePattern ("cached-function");

  boost::shared_ptr<F> f (new F ());

  CachedFunction<DifferentiableFunction> cachedF (f);

  (*output) << cachedF << ":" << std::endl
	    << std::endl;

  Function::vector_t x (2);
  for (double i = 0.; i < 10.; i += 0.5)
  {
    x[0] = i;
    x[1] = 2*i;
    (*output) << cachedF (x) << std::endl;
    (*output) << cachedF (x) << std::endl;
    BOOST_CHECK_EQUAL ((*f) (x)[0], cachedF (x)[0]);

    (*output) << cachedF.gradient (x) << std::endl;
    (*output) << cachedF.gradient (x) << std::endl;

    (*output) << cachedF.jacobian (x) << std::endl;
    (*output) << cachedF.jacobian (x) << std::endl;
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
