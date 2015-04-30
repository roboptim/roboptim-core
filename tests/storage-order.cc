// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

using namespace roboptim;

template <typename T>
struct Dummy : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericDifferentiableFunction<T>);

  Dummy () : parent_t (2, 2, "dummy function")
  {}

  void impl_compute (result_ref res, const_argument_ref x) const
  {
    res[0] = 1. * x[0] + 2. * x[1];
    res[1] = 3. * x[0] + 4. * x[1];
  }

  void impl_gradient (gradient_ref, const_argument_ref, size_type) const
  {
    assert (0);
  }

  void impl_jacobian (jacobian_ref, const_argument_ref x) const;
};

template <>
void Dummy<EigenMatrixSparse>::impl_jacobian
(jacobian_ref jac, const_argument_ref) const
{
  jac.setZero ();
  jac.insert (0,0) = 1.;
  jac.insert (0,1) = 2.;
  jac.insert (1,0) = 3.;
  jac.insert (1,1) = 4.;
}

template <typename T>
void Dummy<T>::impl_jacobian
(jacobian_ref jac, const_argument_ref) const
{
  jac.setZero ();
  jac (0,0) = 1.;
  jac (0,1) = 2.;
  jac (1,0) = 3.;
  jac (1,1) = 4.;
}

template <typename T>
struct Dummy2 : public GenericDifferentiableFunction<T>
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericDifferentiableFunction<T>);

  Dummy2 () : parent_t (2, 1, "dummy function 2")
  {}

  void impl_compute (result_ref res, const_argument_ref x) const
  {
    res[0] = 1. * x[0] + 2. * x[1];
  }

  void impl_gradient (gradient_ref, const_argument_ref, size_type) const
  {
    assert (0);
  }

  void impl_jacobian (jacobian_ref, const_argument_ref x) const;
};

template <>
void Dummy2<EigenMatrixSparse>::impl_jacobian
(jacobian_ref jac, const_argument_ref) const
{
  jac.setZero ();
  jac.insert (0,0) = 1.;
  jac.insert (0,1) = 2.;
}

template <typename T>
void Dummy2<T>::impl_jacobian
(jacobian_ref jac, const_argument_ref) const
{
  jac.setZero ();
  jac (0,0) = 1.;
  jac (0,1) = 2.;
}

boost::shared_ptr<boost::test_tools::output_test_stream> output;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_AUTO_TEST_CASE_TEMPLATE (storage_order, T, functionTypes_t)
{
  output = retrievePattern ("storage-order");

  // Check storage order
  BOOST_CHECK (roboptim::StorageOrder == Eigen::ROBOPTIM_STORAGE_ORDER);

  typedef Dummy<T> function_t;
  typedef typename function_t::gradient_t gradient_t;
  typedef typename function_t::jacobian_t jacobian_t;
  typedef typename function_t::argument_t argument_t;

  typedef DifferentiableFunction::matrix_t denseMatrix_t;
  //typedef DifferentiableFunction::rowVector_t denseRowVector_t;

  Dummy<T> f;
  argument_t x (2);
  x.setZero ();
  // Matrix
  jacobian_t jac = f.jacobian (x);
  (*output) << denseMatrix_t (jac) << std::endl;

  gradient_t grad (2);
  grad.setZero ();
  // Row vector += row of matrix (currently bugged in Eigen 3.2,
  // cf. http://eigen.tuxfamily.org/bz/show_bug.cgi?id=980, fails at runtime)
  //grad += jac.row (0);
  //(*output) << "B = A.row(0) = " << denseRowVector_t (grad) << std::endl;

  Dummy2<T> f2;
  jacobian_t jac2 = 2. * f2.jacobian (x);
  (*output) << denseMatrix_t (jac2) << std::endl;
  // Row vector += matrix of correct size (currently bugged in Eigen 3.2, fails
  // at compile time)
  //grad.setZero ();
  //grad += jac2;
  //(*output) << "D = B + C = " << denseRowVector_t (grad) << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
