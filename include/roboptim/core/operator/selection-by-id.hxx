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

#ifndef ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HXX
# define ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  SelectionById<U>::SelectionById
  (boost::shared_ptr<U> origin,
   std::vector<bool> selector)
    : detail::AutopromoteTrait<U>::T_type
      (origin->inputSize (),
       static_cast<size_type>
       (std::count (selector.begin (), selector.end (), true)),
       (boost::format ("selectionById(%1%)")
	% origin->getName ()).str ()),
      origin_ (origin),
      selector_ (selector),
      result_ (origin->outputSize ()),
      gradient_ (origin->inputSize ())
  {
    result_.setZero ();
    gradient_.setZero ();

    if (selector.size () != static_cast<std::size_t> (origin->outputSize ()))
      {
	boost::format fmt
	  ("selector size is invalid (size is %d but %d was expected)");
	// Ignore some irrelevant exceptions
	fmt.exceptions (boost::io::all_error_bits ^ (boost::io::too_many_args_bit
						     | boost::io::too_few_args_bit
						     | boost::io::bad_format_string_bit));
	fmt % selector.size () % origin->outputSize ();
	throw std::runtime_error (fmt.str ());
      }
  }

  template <typename U>
  SelectionById<U>::~SelectionById ()
  {}

  template <typename U>
  void
  SelectionById<U>::impl_compute
  (result_ref result, const_argument_ref x)
    const
  {
    origin_->operator () (result_, x);

    size_type id = 0;
    for (size_type row = 0; row < result_.size (); ++row)
      if (selector_[static_cast<std::size_t> (row)])
	result[id++] = result_[row];
  }

  // The gradient size depends on the input size which is not varying
  // as we are filtering output but not input here.
  //
  // The only different value is the functionId value which may be lower
  // if some outputs have been filtered out.
  template <typename U>
  void
  SelectionById<U>::impl_gradient (gradient_ref gradient,
				   const_argument_ref argument,
				   size_type functionId)
    const
  {
    size_type functionIdUpdated = functionId;
    for (size_type element = 0; element < functionId; ++element)
      if (selector_[static_cast<std::size_t> (element)])
	functionIdUpdated--;

    origin_->gradient (gradient, argument, functionIdUpdated);
  }

  template <typename U>
  void
  SelectionById<U>::impl_jacobian (jacobian_ref jacobian,
				   const_argument_ref argument)
    const
  {
    size_type row = 0;
    for (size_type functionId = 0;
	 functionId < origin_->outputSize (); ++functionId)
      {
	if (selector_[static_cast<std::size_t> (functionId)])
	  {
	    origin_->gradient (gradient_, argument, functionId);
	    for (size_type col = 0; col < jacobian.cols (); ++col)
	      jacobian.coeffRef (row, col) = gradient_.coeffRef (col);
	    ++row;
	  }
      }
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HXX
