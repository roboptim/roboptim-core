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

#ifndef ROBOPTIM_CORE_FILTER_CHAIN_HXX
# define ROBOPTIM_CORE_FILTER_CHAIN_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U, typename V>
  Chain<U, V>::Chain
  (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
    : detail::PromoteTrait<U, V>::T_promote
      (right->inputSize (),
       left->outputSize (),
       (boost::format ("%1%(%2%)")
	% left->getName ()
	% right->getName ()).str ()),
      left_ (left),
      right_ (right),
      rightResult_ (right->outputSize ()),
      gradientLeft_ (left->inputSize ()),
      gradientRight_ (right->inputSize ()),
      jacobianLeft_ (left->outputSize (),
		     left->inputSize ()),
      jacobianRight_ (right->outputSize (),
		      right->inputSize ())
  {
    if (left->inputSize () != right->outputSize ())
      throw std::runtime_error
	("left input size and right output size mismatch");

    rightResult_.setZero ();
    gradientLeft_.setZero ();
    gradientRight_.setZero ();
    jacobianLeft_.setZero ();
    jacobianRight_.setZero ();
  }

  template <typename U, typename V>
  Chain<U, V>::~Chain ()
  {}

  template <typename U, typename V>
  void
  Chain<U, V>::impl_compute
  (result_ref result, const_argument_ref x)
    const
  {
    (*right_) (rightResult_, x);
    (*left_) (result, rightResult_);
  }

  template <typename U, typename V>
  void
  Chain<U, V>::impl_gradient (gradient_ref gradient,
			 const_argument_ref x,
			 size_type functionId)
    const
  {
    (*right_) (rightResult_, x);
    left_->gradient (gradientLeft_, rightResult_, functionId);
    right_->jacobian (jacobianRight_, x);
    gradient = gradientLeft_.adjoint () * jacobianRight_;
  }

  template <typename U, typename V>
  void
  Chain<U, V>::impl_jacobian (jacobian_ref jacobian,
			      const_argument_ref x)
    const
  {
    (*right_) (rightResult_, x);
    left_->jacobian (jacobianLeft_, rightResult_);
    right_->jacobian (jacobianRight_, x);
    jacobian = jacobianLeft_ * jacobianRight_;
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_CHAIN_HXX
