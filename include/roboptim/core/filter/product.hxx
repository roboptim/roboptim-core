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

#ifndef ROBOPTIM_CORE_FILTER_PRODUCT_HXX
# define ROBOPTIM_CORE_FILTER_PRODUCT_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U, typename V>
  Product<U, V>::Product
  (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
    : detail::PromoteTrait<U, V>::T_promote
      (left->inputSize (),
       left->outputSize (),
       (boost::format ("%1% + %2%")
	% left->getName ()
	% right->getName ()).str ()),
      left_ (left),
      right_ (right),
      resultLeft_ (left->outputSize ()),
      resultRight_ (left->outputSize ()),
      gradientLeft_ (left->inputSize ()),
      gradientRight_ (left->inputSize ()),
      jacobianLeft_ (left->outputSize (),
		     left->inputSize ()),
      jacobianRight_ (left->outputSize (),
		      left->inputSize ())
  {
    if (left->inputSize () != right->inputSize ()
	|| left->outputSize () != right->outputSize ())
      throw std::runtime_error ("left and right size mismatch");
    resultLeft_.setZero ();
    resultRight_.setZero ();
    gradientLeft_.setZero ();
    gradientRight_.setZero ();
    jacobianLeft_.setZero ();
    jacobianRight_.setZero ();
  }

  template <typename U, typename V>
  Product<U, V>::~Product ()
  {}

  template <typename U, typename V>
  void
  Product<U, V>::impl_compute
  (result_t& result, const argument_t& x)
    const
  {
    (*left_) (resultLeft_, x);
    (*right_) (resultRight_, x);
    result.noalias () = resultLeft_.cwiseProduct (resultRight_);
  }

  template <typename U, typename V>
  void
  Product<U, V>::impl_gradient (gradient_t& gradient,
				const argument_t& argument,
				size_type functionId)
    const
  {
    left_->gradient (gradientLeft_, argument, functionId);
    right_->gradient (gradientRight_, argument, functionId);
    gradient = gradientLeft_.cwiseProduct (gradientRight_);
  }

  template <typename U, typename V>
  void
  Product<U, V>::impl_jacobian (jacobian_t& jacobian,
				const argument_t& argument)
    const
  {
    left_->jacobian (jacobianLeft_, argument);
    right_->jacobian (jacobianRight_, argument);
    jacobian = jacobianLeft_ * jacobianRight_;
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_PRODUCT_HXX
