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

#ifndef ROBOPTIM_CORE_FILTER_SELECTION_HXX
# define ROBOPTIM_CORE_FILTER_SELECTION_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  Selection<U>::Selection
  (boost::shared_ptr<U> origin,
   size_type start,
   size_type size)
    : detail::AutopromoteTrait<U>::T_type
      (origin->inputSize (),
       size,
       (boost::format ("selection(%1%)")
	% origin->getName ()).str ()),
      origin_ (origin),
      start_ (start),
      size_ (size),
      result_ (origin->outputSize ()),
      jacobian_ (origin->outputSize (),
		 origin->inputSize ())
  {
    if (start + size > origin->inputSize ())
      throw std::runtime_error ("invalid start/size");

    result_.setZero ();
    gradient_.setZero ();
    jacobian_.setZero ();
  }

  template <typename U>
  Selection<U>::~Selection ()
  {}

  template <typename U>
  void
  Selection<U>::impl_compute
  (result_t& result, const argument_t& x)
    const
  {
    origin_->operator () (result_, x);
    result = result_.segment (start_, size_);
  }

  template <typename U>
  void
  Selection<U>::impl_gradient (gradient_t& gradient,
			 const argument_t& argument,
			 size_type functionId)
    const
  {
    origin_->gradient (gradient, argument, functionId);
  }

  template <typename U>
  void
  Selection<U>::impl_jacobian (jacobian_t& jacobian,
			 const argument_t& argument)
    const
  {
    origin_->jacobian (jacobian_, argument);
    jacobian = jacobian_.block (start_, 0, size_, jacobian_.cols ());
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_SELECTION_HXX
