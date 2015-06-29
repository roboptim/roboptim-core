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

#ifndef ROBOPTIM_CORE_NUMERIC_QUADRATIC_FUNCTION_HXX
# define ROBOPTIM_CORE_NUMERIC_QUADRATIC_FUNCTION_HXX

# include <roboptim/core/indent.hh>
# include <roboptim/core/numeric-linear-function.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  template <typename T>
  GenericNumericQuadraticFunction<T>::GenericNumericQuadraticFunction
  (const_matrix_ref a, const_vector_ref b)
    : GenericQuadraticFunction<T>
      (a.rows (), 1, "numeric quadratic function"),
      a_ (a),
      b_ (b),
      c_ (1),
      buffer_ (b.size ())
  {
    assert (b.size () == this->inputSize ());
    c_.setZero ();
  }

  template <typename T>
  GenericNumericQuadraticFunction<T>::GenericNumericQuadraticFunction
  (const_matrix_ref a, const_vector_ref b, const_vector_ref c)
    : GenericQuadraticFunction<T>
      (a.rows (), 1, "numeric quadratic function"),
      a_ (a),
      b_ (b),
      c_ (c),
      buffer_ (b.size ())
  {
    assert (b.size () == this->inputSize ());
    assert (c.size () == 1);
  }

  template <typename T>
  GenericNumericQuadraticFunction<T>::~GenericNumericQuadraticFunction ()
  {
  }

  // x^T * A * x + b^T * x + c
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_compute (result_ref result,
						    const_argument_ref argument)
    const
  {
    buffer_.noalias () = a_ * argument;
    result = argument.adjoint ()  * buffer_;
    result += b_.adjoint () * argument;
    result += c_;
  }

  // 2 * x * A + b
  template <>
  inline void
  GenericNumericQuadraticFunction<EigenMatrixSparse>::impl_jacobian
  (jacobian_ref jacobian, const_argument_ref x) const
  {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    bool cur_malloc_allowed = is_malloc_allowed ();
    set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    Eigen::MatrixXd j = 2 * x.transpose () * a_;

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    for (size_type i = 0; i < this->inputSize (); ++i)
      j.coeffRef (0, i) += b_[i];

    for (size_type i = 0; i < this->inputSize (); ++i)
      jacobian.coeffRef (0, i) = j.coeffRef (0, i);;
  }


  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_jacobian
  (jacobian_ref jacobian, const_argument_ref x) const
  {
    jacobian.noalias () = 2 * x.transpose () * a_;
    jacobian += b_.transpose ();
  }

  // A(i) - sparse specialization
  template <>
  inline void
  GenericNumericQuadraticFunction<EigenMatrixSparse>::impl_gradient
  (gradient_ref gradient, const_argument_ref x, size_type)
    const
  {
    buffer_.noalias () = 2 * a_ * x;
    buffer_ += b_;
    for (size_type j = 0; j < this->inputSize (); ++j)
      gradient.coeffRef (j) = buffer_.coeffRef (j);
  }

  // A(i)
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_gradient
  (gradient_ref gradient, const_argument_ref x, size_type) const
  {
    buffer_.noalias () = 2 * a_ * x;
    buffer_ += b_;
    gradient = buffer_;
  }

  // A
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_hessian
  (hessian_ref hessian, const_argument_ref, size_type) const
  {
    hessian = a_;
  }

  template <typename T>
  std::ostream&
  GenericNumericQuadraticFunction<T>::print (std::ostream& o) const
  {
    return o << "Numeric quadratic function" << incindent << iendl
             << "A = " << this->a_ << iendl
             << "B = " << this->b_ << iendl
	     << "c = " << this->c_
             << decindent;
  }

  template <typename T>
  const unsigned long GenericNumericQuadraticFunction<T>::flag_ = ROBOPTIM_IS_NUMERIC_QUADRATIC | GenericNumericQuadraticFunction<T>::parent_t::flag_;
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HXX
