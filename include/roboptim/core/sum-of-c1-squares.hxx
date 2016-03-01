
// Copyright (c) 2011 CNRS
// Authors: Florent Lamiraux
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


#ifndef ROBOPTIM_CORE_SUM_OF_C1_SQUARES_HXX
# define ROBOPTIM_CORE_SUM_OF_C1_SQUARES_HXX

namespace roboptim {

  template <typename T>
  GenericSumOfC1Squares<T>::GenericSumOfC1Squares (const boost::shared_ptr<parent_t>&
                                                   function,
                                                   const std::string& name) :
    parent_t (function->inputSize(), 1, name),
    baseFunction_ (function)
  {
    value_.resize (function->outputSize());
    gradient_.resize (function->inputSize());
    x_.resize (function->inputSize());
    x_.setZero ();
    (*baseFunction_) (value_, x_);
  }

  template <typename T>
  GenericSumOfC1Squares<T>::GenericSumOfC1Squares (const GenericSumOfC1Squares<T>& src):
    parent_t (src.inputSize(), 1, src.getName()),
    baseFunction_ (src.baseFunction_), x_ (src.x_),
    value_ (src.value_),
    gradient_ (src.gradient_)
  {
  }

  template <typename T>
  GenericSumOfC1Squares<T>::~GenericSumOfC1Squares ()
  {
  }

  template <typename T>
  const boost::shared_ptr<const typename GenericSumOfC1Squares<T>::parent_t>&
  GenericSumOfC1Squares<T>::baseFunction () const
  {
    return baseFunction_;
  }

  template <typename T>
  void GenericSumOfC1Squares<T>::
  impl_compute(result_ref result, const_argument_ref x) const
  {
    computeFunction (x);
    value_type sumSquares = 0;
    for (typename result_t::Index i = 0; i < value_.size(); i++) {
      value_type y = value_[i];
      sumSquares += y*y;
    }
    result[0] = sumSquares;
  }

  template <typename T>
  void GenericSumOfC1Squares<T>::
  impl_gradient(gradient_ref gradient, const_argument_ref x,
                size_type ROBOPTIM_DEBUG_ONLY (row)) const
  {
    assert (row == 0);
    computeFunction (x);
    gradient.setZero ();
    for (typename result_t::Index i = 0; i < value_.size (); i++) {
      value_type y = value_[i];
      gradient_.setZero ();
      baseFunction_->gradient (gradient_, x, static_cast<size_type> (i));
      gradient += 2*y*gradient_;
    }
  }

  template <typename T>
  void GenericSumOfC1Squares<T>::computeFunction (const_argument_ref x) const
  {
    if (x != x_) {
      x_ = x;
      (*baseFunction_) (value_, x_);
    }
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  extern template class ROBOPTIM_DLLAPI GenericSumOfC1Squares<EigenMatrixDense>;
  extern template class ROBOPTIM_DLLAPI GenericSumOfC1Squares<EigenMatrixSparse>;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

} // namespace roboptim

#endif //! ROBOPTIM_CORE_SUM_OF_C1_SQUARES_HXX
