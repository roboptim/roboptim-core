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

#ifndef ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HXX
# define ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  SelectionById<U>::SelectionById
  (boost::shared_ptr<U> origin,
   std::vector<bool> selector) throw (std::runtime_error)
    : detail::AutopromoteTrait<U>::T_type
      (origin->inputSize (),
       static_cast<size_type>
       (std::count (selector.begin (), selector.end (), true)),
       (boost::format ("selectionById(%1%)")
	% origin->getName ()).str ()),
      origin_ (origin),
      selector_ (selector),
      result_ (origin->outputSize ()),
      gradient_ (origin->inputSize ()),
      jacobian_ (origin->outputSize (),
		 origin->inputSize ())
  {
    result_.setZero ();
    gradient_.setZero ();
    jacobian_.setZero ();
  }

  template <typename U>
  SelectionById<U>::~SelectionById () throw ()
  {}

  template <typename U>
  void
  SelectionById<U>::impl_compute
  (result_t& result, const argument_t& x)
    const throw ()
  {
    origin_->operator () (result_, x);

    size_type id = 0;
    for (size_type row = 0; row < result_.size (); ++row)
      if (selector_[static_cast<std::size_t> (row)])
	result[id++] = result_[row];
  }

  template <typename U>
  void
  SelectionById<U>::impl_gradient (gradient_t& gradient,
				   const argument_t& argument,
				   size_type functionId)
    const throw ()
  {
    origin_->gradient (gradient_, argument, functionId);

    size_type id = 0;
    for (size_type row = 0; row < gradient_.size (); ++row)
      if (selector_[static_cast<std::size_t> (row)])
	gradient.coeffRef (id++) = gradient_.coeffRef (row);
  }

  template <typename U>
  void
  SelectionById<U>::impl_jacobian (jacobian_t& jacobian,
				   const argument_t& argument)
    const throw ()
  {
    origin_->jacobian (jacobian_, argument);

    size_type id = 0;
    for (size_type row = 0; row < jacobian_.rows (); ++row)
      if (selector_[static_cast<std::size_t> (row)])
	{
	  for (size_type col = 0; col < jacobian_.cols (); ++col)
	    jacobian.coeffRef (id, col) = jacobian_.coeffRef (row, col);
	  ++id;
	}
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HXX
