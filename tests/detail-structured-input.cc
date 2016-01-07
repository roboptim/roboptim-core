// Copyright (C) 2015 by Grégoire Duchemin, AIST, CNRS, EPITA
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

#include <roboptim/core/sys.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/detail/structured-input.hh>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

typedef boost::mpl::list< ::roboptim::EigenMatrixDense
			  ,::roboptim::EigenMatrixSparse
			  > functionTypes_t;

template<class T>
struct F : public roboptim::GenericDifferentiableFunction<T>,
	   public roboptim::detail::StructuredInput<roboptim::GenericDifferentiableFunction<T> >
{
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (roboptim::GenericDifferentiableFunction<T>);

  F () : roboptim::GenericDifferentiableFunction<T> (22, 10, "f_n (x) = n * x")
  {
    this->addBlock(5);
    this->addBlock(12);
    this->addBlock(5);
  }

  void impl_compute (result_ref res, const_argument_ref argument) const
  {
    res.setZero ();
    for (size_type i = 0; i < this->outputSize (); ++i)
      for (size_type j = 0; j < 3; ++j)
	{
	  res[i] += (value_type)i * argument[19 + j];
	}
  }

  void impl_gradient (gradient_ref, const_argument_ref,
		      size_type) const
  {
  }

  void impl_jacobian (jacobian_ref jac, const_argument_ref input) const
  {
    for(int i = 0; i < this->outputSize(); ++i)
      {
	this->getJacobianBlock(jac, 0).row(i) << this->getInputBlock(input, 2).transpose();
	this->getJacobianBlock(jac, 1).row(i) << this->getInputBlock(input, 1).transpose();
	this->getJacobianBlock(jac, 2).row(i) << this->getInputBlock(input, 0).transpose();
      }
  }

};

template <>
inline void
F<roboptim::EigenMatrixSparse>::impl_gradient(gradient_ref grad, const_argument_ref, size_type functionId) const
{
  grad.setZero ();
  for (size_type j = 0; j < 3; ++j)
    {
      grad.insert (19 + j) += (value_type)functionId;
    }
}

template <>
inline void
F<roboptim::EigenMatrixSparse>::impl_jacobian (jacobian_ref jac, const_argument_ref input) const
  {
    for(int i = 0; i < this->outputSize(); ++i)
      {
	for(int j = 0; j < 5; ++j)
	  {
	    this->getJacobianBlock(jac, 0).coeffRef(i, j) = this->getInputBlock(input, 2)[j];
	  }

	for(int j = 5; j < 12 + 5; ++j)
	  {
	    this->getJacobianBlock(jac, 0).coeffRef(i, j) = this->getInputBlock(input, 1)[j - 5];
	  }

	for(int j = 12 + 5; j < 5 + 12 + 5; ++j)
	  {
	    this->getJacobianBlock(jac, 0).coeffRef(i, j) = this->getInputBlock(input, 0)[j - (12 + 5)];
	  }


      }
  }

template<class T>
struct G : public roboptim::GenericFunction<T>,
	   public roboptim::detail::StructuredInput<roboptim::GenericFunction<T> >
{
  ROBOPTIM_FUNCTION_FWD_TYPEDEFS_
  (roboptim::GenericFunction<T>);

  G () : roboptim::GenericFunction<T> (22, 10, "g_n (x) = n * x")
  {
    this->addBlock(5);
    this->addBlock(12);
    this->addBlock(5);
  }

  void impl_compute (result_ref res, const_argument_ref argument) const
  {
    res.setZero ();
    for (size_type i = 0; i < this->outputSize (); ++i)
      for (size_type j = 0; j < 3; ++j)
	{
	  res[i] += (value_type)i * argument[19 + j];
	}
  }

};

boost::shared_ptr<boost::test_tools::output_test_stream> output;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (detail_structured_input, T, functionTypes_t)
{
  F<T> f;

  // Note: here we use ROBOPTIM_UNUSED to prevent compiler warnings
  // For more information, cf. https://svn.boost.org/trac/boost/ticket/7242
  // F is a DifferentiableFunction, and as such exposes the getJacobianBlock() method
  BOOST_STATIC_ASSERT((boost::is_base_of<roboptim::detail::StructuredInputJacobianInternal<typename F<T>::parent_t, typename F<T>::traits_t>, F<T> >::value))
# if __cplusplus < 201103L || !defined (__GXX_EXPERIMENTAL_CXX0X__)
 ROBOPTIM_UNUSED;
# else
  ;
# endif
  // G is just a Function, and as such does not expose the getJacobianBlock() method
  BOOST_STATIC_ASSERT((!boost::is_base_of<roboptim::detail::StructuredInputJacobianInternal<typename G<T>::parent_t, typename G<T>::traits_t>, G<T> >::value))
# if __cplusplus < 201103L || !defined (__GXX_EXPERIMENTAL_CXX0X__)
 ROBOPTIM_UNUSED;
# else
  ;
# endif

  typename F<T>::argument_t arg(22);
  for(int i = 0; i < arg.size(); ++i)
    {
      arg(i) = i + 1;
    }

  typename F<T>::jacobian_t jacobian(10, 22);
  jacobian.setZero();

  f.jacobian(jacobian, arg);

  Eigen::MatrixXd denseJac(jacobian);

  for (int i = 0; i < f.outputSize(); ++i)
    {
      int j = 0;

      for(; j < 5; ++j)
	{
	  BOOST_CHECK(denseJac(i, j) == 12 + 5 + 1 + j);
	}

      for(; j < 5 + 12; ++j)
	{
	  BOOST_CHECK(denseJac(i, j) == j + 1);
	}

      for(; j < 5 + 12 + 5; ++j)
	{
	  BOOST_CHECK(denseJac(i, j) == j - (12 + 5) + 1);
	}

    }


}

BOOST_AUTO_TEST_SUITE_END ()
