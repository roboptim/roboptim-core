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

#ifndef ROBOPTIM_CORE_FILTER_PLUS_HXX
# define ROBOPTIM_CORE_FILTER_PLUS_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U, typename V>
  Plus<U, V>::Plus
  (boost::shared_ptr<U> left, boost::shared_ptr<V> right) throw ()
    : detail::PromoteTrait<U, V>::T_promote
      (left->inputSize (),
       left->outputSize (),
       (boost::format ("%1% + %2%")
	% left->getName ()
	% right->getName ()).str ()),
      left_ (left),
      right_ (right),
      result_ (left->outputSize ()),
      gradient_ (left->inputSize ()),
      jacobian_ (left->outputSize (),
		 left->inputSize ())
  {
    if (left->inputSize () != right->inputSize ()
	|| left->outputSize () != right->outputSize ())
      throw std::runtime_error ("left and right size mismatch");
    result_.setZero ();
    gradient_.setZero ();
    jacobian_.setZero ();
  }

  template <typename U, typename V>
  Plus<U, V>::~Plus () throw ()
  {}

  template <typename U, typename V>
  void
  Plus<U, V>::impl_compute
  (result_t& result, const argument_t& x)
    const throw ()
  {
    result.setZero ();
    (*left_) (result, x);
    (*right_) (result_, x);
    result += result_;
  }

  template <typename U, typename V>
  void
  Plus<U, V>::impl_gradient (gradient_t& gradient,
			 const argument_t& argument,
			 size_type functionId)
    const throw ()
  {
    left_->gradient (gradient, argument, functionId);
    right_->gradient (gradient_, argument, functionId);
    gradient += gradient_;
  }

  template <typename U, typename V>
  void
  Plus<U, V>::impl_jacobian (jacobian_t& jacobian,
			 const argument_t& argument)
    const throw ()
  {
    left_->jacobian (jacobian, argument);
    right_->jacobian (jacobian_, argument);
    jacobian += jacobian_;
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_PLUS_HXX
