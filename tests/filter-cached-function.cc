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

struct DenseF : public DifferentiableFunction
{
  DenseF (bool verbose = true)
    : DifferentiableFunction (2, 1, "2 * x * x + y"),
      verbose_ (verbose)
  {}

  void impl_compute (result_ref res, const_argument_ref argument) const throw ()
  {
    if (verbose_)
      (*output) << "computation (not cached)" << std::endl;
    res.setZero ();
    res[0] = 2. * argument[0] * argument[0] + argument[1];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref argument,
		      size_type) const
  {
    if (verbose_)
      (*output) << "gradient computation (not cached)" << std::endl;
    grad.setZero ();
    grad[0] = 4. * argument[0];
    grad[1] = 1.;
  }

  void impl_jacobian (jacobian_ref jacobian, const_argument_ref argument)
    const
  {
    if (verbose_)
      (*output) << "jacobian computation (not cached)" << std::endl;
    jacobian.setZero ();
    jacobian(0,0) = 4. * argument[0];
    jacobian(0,1) = 1.;
  }

private:
  bool verbose_;
};

struct SparseF : public DifferentiableSparseFunction
{
  SparseF (bool verbose = true)
    : DifferentiableSparseFunction (2, 1, "2 * x * x + y"),
      verbose_ (verbose)
  {}

  void impl_compute (result_ref res, const_argument_ref argument) const throw ()
  {
    if (verbose_)
      (*output) << "computation (not cached)" << std::endl;
    res.setZero ();
    res[0] = 2. * argument[0] * argument[0] + argument[1];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref argument,
                      size_type) const
  {
    if (verbose_)
      (*output) << "gradient computation (not cached)" << std::endl;
    grad.setZero ();
    grad.insert(0) = 4. * argument[0];
    grad.insert(1) = 1.;
  }

  void impl_jacobian (jacobian_ref jacobian, const_argument_ref argument)
    const
  {
    if (verbose_)
      (*output) << "jacobian computation (not cached)" << std::endl;
    jacobian.setZero ();
    jacobian.insert(0,0) = 4. * argument[0];
    jacobian.insert(0,1) = 1.0;
  }

private:
  bool verbose_;
};


template <typename U1, typename U2, typename V1, typename V2>
void loopCachedFunction
(boost::shared_ptr<U1>& denseF, CachedFunction<U2>& cachedDenseF,
 boost::shared_ptr<V1>& sparseF, CachedFunction<V2>& cachedSparseF,
 Function::vector_t& x, unsigned n_iter)
{
  double max_value = 100;
  double step = max_value/n_iter;
  for (double i = 0.; i < max_value; i+=step)
    {
      x[0] = i;
      x[1] = 2*i;
      cachedDenseF (x);
      cachedDenseF (x);
      cachedSparseF (x);
      cachedSparseF (x);
      BOOST_CHECK_EQUAL ((*denseF) (x)[0], cachedDenseF (x)[0]);
      BOOST_CHECK_EQUAL ((*sparseF) (x)[0], cachedSparseF (x)[0]);
    }
}

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE (cached_function)
{
  output = retrievePattern ("filter-cached-function");

  // First: test dense cached function
  boost::shared_ptr<DenseF> dense_f (new DenseF ());

  CachedFunction<DifferentiableFunction> cachedDenseF (dense_f);

  (*output) << cachedDenseF << ":" << std::endl
	    << std::endl;

  Function::vector_t x (2);
  for (double i = 0.; i < 10.; i += 0.5)
    {
      x[0] = i;
      x[1] = 2*i;
      (*output) << cachedDenseF (x) << std::endl;
      (*output) << cachedDenseF (x) << std::endl;
      BOOST_CHECK_EQUAL ((*dense_f) (x)[0], cachedDenseF (x)[0]);

      (*output) << cachedDenseF.gradient (x) << std::endl;
      (*output) << cachedDenseF.gradient (x) << std::endl;

      (*output) << cachedDenseF.jacobian (x) << std::endl;
      (*output) << cachedDenseF.jacobian (x) << std::endl;
    }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern (true));

  // Second: test sparse cached function
  output = retrievePattern ("filter-cached-function");
  boost::shared_ptr<SparseF> sparse_f (new SparseF ());

  CachedFunction<DifferentiableSparseFunction> cachedSparseF (sparse_f);

  (*output) << cachedSparseF << ":" << std::endl
            << std::endl;

  for (double i = 0.; i < 10.; i += 0.5)
    {
      x[0] = i;
      x[1] = 2*i;
      (*output) << cachedSparseF (x) << std::endl;
      (*output) << cachedSparseF (x) << std::endl;
      BOOST_CHECK_EQUAL ((*sparse_f) (x)[0], cachedSparseF (x)[0]);

      (*output) << sparse_to_dense (cachedSparseF.gradient (x)) << std::endl;
      (*output) << sparse_to_dense (cachedSparseF.gradient (x)) << std::endl;

      (*output) << sparse_to_dense (cachedSparseF.jacobian (x)) << std::endl;
      (*output) << sparse_to_dense (cachedSparseF.jacobian (x)) << std::endl;
    }

  // More tests for memory usage (disabling verbose mode)
  dense_f = boost::shared_ptr<DenseF> (new DenseF (false));
  sparse_f = boost::shared_ptr<SparseF> (new SparseF (false));
  cachedDenseF = CachedFunction<DifferentiableFunction> (dense_f);
  cachedSparseF = CachedFunction<DifferentiableSparseFunction> (sparse_f);

  // Loop over some values
  loopCachedFunction (dense_f, cachedDenseF, sparse_f, cachedSparseF, x, 1e5);

  // Reset and retry
  cachedDenseF.reset ();
  cachedSparseF.reset ();
  loopCachedFunction (dense_f, cachedDenseF, sparse_f, cachedSparseF, x, 1e5);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
