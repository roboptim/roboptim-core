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

#ifndef ROBOPTIM_CORE_FILTER_CONCATENATE_HXX
# define ROBOPTIM_CORE_FILTER_CONCATENATE_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  Concatenate<U>::Concatenate
  (boost::shared_ptr<U> left,
   boost::shared_ptr<U> right) throw ()
    : detail::AutopromoteTrait<U>::T_type
      (left->inputSize (),
       left->outputSize () + right->outputSize (),
       (boost::format ("concatenate(%1%, %2%)")
	% left->getName ()
	% right->getName ()).str ()),
      left_ (left),
      right_ (right),
      resultLeft_ (left->outputSize ()),
      resultRight_ (right->outputSize ()),
      jacobianLeft_ (left->outputSize (),
		     left->inputSize ()),
      jacobianRight_ (right->outputSize (),
		      right->inputSize ())

  {
    if (left->inputSize () != right->inputSize ())
      throw std::runtime_error ("left and right input size are not the same");

    resultLeft_.setZero ();
    resultRight_.setZero ();
    jacobianLeft_.setZero ();
    jacobianRight_.setZero ();
  }

  template <typename U>
  Concatenate<U>::~Concatenate () throw ()
  {}

  template <typename U>
  void
  Concatenate<U>::impl_compute
  (result_t& result, const argument_t& x)
    const throw ()
  {
    left_->operator () (resultLeft_, x);
    right_->operator () (resultRight_, x);
    result.segment (0, left_->outputSize ()) = resultLeft_;
    result.segment (left_->inputSize (), right_->outputSize ()) = resultRight_;
  }

  template <typename U>
  void
  Concatenate<U>::impl_gradient (gradient_t& gradient,
				 const argument_t& x,
				 size_type functionId)
    const throw ()
  {
    if (functionId < left_->outputSize ())
      left_->gradient (gradient, x, functionId);
    else
      right_->gradient (gradient, x, functionId - left_->outputSize ());
  }

  template <typename U>
  void
  Concatenate<U>::impl_jacobian (jacobian_t& jacobian,
				 const argument_t& x)
    const throw ()
  {
    left_->jacobian (jacobianLeft_, x);
    right_->jacobian (jacobianRight_, x);
    jacobian.middleRows (0, left_->outputSize ()) =
      jacobianLeft_;
    jacobian.middleRows (left_->outputSize (), right_->outputSize ()) =
      jacobianRight_;
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_CONCATENATE_HXX
