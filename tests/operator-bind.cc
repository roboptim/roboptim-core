// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#include <boost/mpl/list.hpp>

#include "shared-tests/fixture.hh"

#include <boost/test/test_case_template.hpp>

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/operator/bind.hh>

#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/function/constant.hh>
#include <roboptim/core/function/identity.hh>

using namespace roboptim;

template <typename T>
struct F : public GenericDifferentiableFunction<T>
{
public:
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  F (size_type n) : GenericDifferentiableFunction<T> (n, 1, "f(x) = Σx_i²")
  {}

  void impl_compute (result_ref res, const_argument_ref x) const
  {
    res[0] = x.squaredNorm ();
  }

  void impl_gradient (gradient_ref grad, const_argument_ref x,
		      size_type) const;
};

template <>
void F<EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type) const
{
  grad.setZero ();
  for (int i = 0; i < inputSize (); ++i)
    {
      grad.insert (i) = 2. * x[i];
    }
}

template <typename T>
void F<T>::impl_gradient
(gradient_ref grad, const_argument_ref x, size_type) const
{
  grad = 2. * x;
}


typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (chain_test, T, functionTypes_t)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("operator-bind");

  typedef Function::vector_t denseVector_t;
  typedef Function::matrix_t denseMatrix_t;

  typedef GenericDifferentiableFunction<T> differentiableFunction_t;
  typedef GenericIdentityFunction<T> identityFunction_t;
  typedef GenericLinearFunction<T> linearFunction_t;

  typedef typename identityFunction_t::value_type value_type;
  typedef typename identityFunction_t::size_type size_type;
  size_type n = 5;

  typename identityFunction_t::result_t offset (n);
  offset.setZero ();

  boost::shared_ptr<identityFunction_t> identity =
    boost::make_shared<identityFunction_t> (offset);

  {
    std::vector<boost::optional<value_type> > boundValues
      (static_cast<size_t> (n), boost::optional<value_type> ());
    boost::shared_ptr<linearFunction_t>
      fct = roboptim::bind (identity, boundValues);

    BOOST_CHECK (fct->inputSize () == n);

    typename identityFunction_t::vector_t x (n);
    x.setZero ();
    (*output)
      << "x: "           << x << "\n"
      << "fct: "         << (*fct) << "\n"
      << "fct(x): "      << (*fct) (x) << "\n"
      << "gradient(x): " << denseVector_t (fct->gradient (x, 0)) << "\n"
      << "jacobian(x): " << denseMatrix_t (fct->jacobian (x)) << std::endl;
  }

  {
    std::vector<boost::optional<value_type> > boundValues
      (static_cast<size_t> (n), boost::optional<value_type> ());
    boundValues[0] = 42.;
    boost::shared_ptr<linearFunction_t>
      fct = roboptim::bind (identity, boundValues);

    BOOST_CHECK (fct->inputSize () == n-1);

    typename identityFunction_t::vector_t x (n-1);
    x.setZero ();
    (*output)
      << "x: "           << x << "\n"
      << "fct: "         << (*fct) << "\n"
      << "fct(x): "      << (*fct) (x) << "\n"
      << "gradient(x): " << denseVector_t (fct->gradient (x, 0)) << "\n"
      << "jacobian(x): " << denseMatrix_t (fct->jacobian (x)) << std::endl;

    std::vector<boost::optional<value_type> > boundValues_throw
      (6, boost::optional<value_type> ());
    boundValues_throw[0] = 42.;
    BOOST_CHECK_THROW (fct = roboptim::bind (identity, boundValues_throw),
                       std::runtime_error);
  }

  {
    std::vector<boost::optional<value_type> > boundValues
      (static_cast<size_t> (n), boost::optional<value_type> ());
    boundValues[1] = 12.;
    boundValues[3] = -2.;

    boost::shared_ptr<F<T> > norm = boost::make_shared<F<T> > (n);
    boost::shared_ptr<differentiableFunction_t>
      fct = boost::make_shared<Bind<differentiableFunction_t> >
      (norm, boundValues);

    BOOST_CHECK (fct->inputSize () == n-2);

    typename differentiableFunction_t::vector_t x (n-2);
    x.fill (1.);
    (*output)
      << "x: "           << x << "\n"
      << "fct: "         << (*fct) << "\n"
      << "fct(x): "      << (*fct) (x) << "\n"
      << "gradient(x): " << denseVector_t (fct->gradient (x, 0)) << "\n"
      << "jacobian(x): " << denseMatrix_t (fct->jacobian (x)) << std::endl;
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
