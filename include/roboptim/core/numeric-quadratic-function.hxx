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
# include "debug.hh"

# include <roboptim/core/indent.hh>
# include <roboptim/core/numeric-linear-function.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  template <typename T>
  GenericNumericQuadraticFunction<T>::GenericNumericQuadraticFunction
  (const matrix_t& a, const vector_t& b)
    throw ()
    : GenericQuadraticFunction<T>
      (a.rows (), 1, "numeric linear function"),
      a_ (a),
      b_ (b),
      buffer_ (b.size ())
  {
    assert (b.size () == this->inputSize ());
  }

  template <typename T>
  GenericNumericQuadraticFunction<T>::~GenericNumericQuadraticFunction () throw ()
  {
  }

  // 1/2 * x^T * A * x + b^T * x
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_compute (result_t& result,
						 const argument_t& argument)
    const throw ()
  {
    buffer_.noalias () = a_ * argument;
    result = .5 * argument.adjoint ()  * buffer_;
    result += b_.adjoint () * argument;
  }

  // x * A + b
  template <>
  inline void
  GenericNumericQuadraticFunction<EigenMatrixSparse>::impl_jacobian
  (jacobian_t& jacobian, const argument_t& x) const throw ()
  {
    jacobian = x.transpose () * a_.selfadjointView<Eigen::Upper> ();
    for (size_type j = 0; j < this->inputSize (); ++j)
      jacobian.coeffRef (0, j) += b_[j];
  }


  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_jacobian
  (jacobian_t& jacobian, const argument_t& x) const throw ()
  {
    jacobian.noalias () = x.transpose () * a_;
    jacobian += b_.transpose ();
  }

  // A(i) - sparse specialization
  template <>
  inline void
  GenericNumericQuadraticFunction<EigenMatrixSparse>::impl_gradient
  (gradient_t& gradient, const argument_t& x, size_type)
    const throw ()
  {
    buffer_.noalias () = a_ * x;
    buffer_ += b_;
    for (size_type j = 0; j < this->inputSize (); ++j)
      gradient.coeffRef (j) = buffer_.coeffRef (j);
  }

  // A(i)
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_gradient
  (gradient_t& gradient, const argument_t& x, size_type) const throw ()
  {
    gradient.noalias () = a_ * x;
    gradient += b_;
  }

  // A
  template <typename T>
  void
  GenericNumericQuadraticFunction<T>::impl_hessian
  (hessian_t& hessian, const argument_t&, size_type) const throw ()
  {
    hessian = a_;
  }

  template <typename T>
  std::ostream&
  GenericNumericQuadraticFunction<T>::print (std::ostream& o) const throw ()
  {
    return o << "Numeric quadratic function" << incindent << iendl
             << "A = " << this->a_ << iendl
             << "B = " << this->b_
             << decindent;
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HXX
