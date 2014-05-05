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

#ifndef ROBOPTIM_CORE_FILTER_MAP_HXX
# define ROBOPTIM_CORE_FILTER_MAP_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  Map<U>::Map
  (boost::shared_ptr<U> origin,
   size_type repeat)
    : detail::AutopromoteTrait<U>::T_type
      (origin->inputSize () * repeat,
       origin->outputSize () * repeat,
       (boost::format ("map(%1%, %2%)")
	% origin->getName ()
	% repeat).str ()),
      origin_ (origin),
      repeat_ (repeat),
      x_ (origin->inputSize ()),
      result_ (origin->outputSize ()),
      gradient_ (origin->inputSize ()),
      jacobian_ (origin->outputSize (),
		 origin->inputSize ())
  {
    x_.setZero ();
    result_.setZero ();
    gradient_.setZero ();
    jacobian_.setZero ();
  }

  template <typename U>
  Map<U>::~Map ()
  {}

  template <typename U>
  void
  Map<U>::impl_compute
  (result_t& result, const argument_t& x)
    const
  {
    for (size_type i = 0; i < repeat_; ++i)
      {
	x_ = x.segment (i * origin_->inputSize (), origin_->inputSize ());
	origin_->operator () (result_, x_);
	result.segment (i * origin_->outputSize (), origin_->outputSize ()) =
	  result_;
      }
  }

  template <typename U>
  void
  Map<U>::impl_gradient (gradient_t& gradient,
			 const argument_t& x,
			 size_type functionId)
    const
  {
    for (size_type i = 0; i < repeat_; ++i)
      {
	x_ = x.segment (i * origin_->inputSize (), origin_->inputSize ());
	origin_->gradient (gradient_, x_, functionId);

	//FIXME: should be a segment but Eigen support is still preliminary.
	for (size_type idx = 0; idx < origin_->inputSize (); ++idx)
	  gradient.coeffRef (i * origin_->inputSize () + idx) =
	    gradient_.coeffRef (idx);
      }
  }

  template <typename U>
  void
  Map<U>::impl_jacobian (jacobian_t& jacobian,
			 const argument_t& x)
    const
  {
    for (size_type i = 0; i < repeat_; ++i)
      {
	x_ = x.segment (i * origin_->inputSize (), origin_->inputSize ());
	origin_->jacobian (jacobian_, x_);

	//FIXME: should be a block but Eigen support is still preliminary.
	for (size_type idx_i = 0; idx_i < origin_->outputSize (); ++idx_i)
	  for (size_type idx_j = 0; idx_j < origin_->inputSize (); ++idx_j)
	    jacobian.coeffRef
	      (i * origin_->outputSize () + idx_i,
	       i * origin_->inputSize () + idx_j) =
	      jacobian_.coeffRef (idx_i, idx_j);
      }
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_MAP_HXX
