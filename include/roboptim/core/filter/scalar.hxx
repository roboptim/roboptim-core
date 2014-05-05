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

#ifndef ROBOPTIM_CORE_FILTER_SCALAR_HXX
# define ROBOPTIM_CORE_FILTER_SCALAR_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename U>
  Scalar<U>::Scalar
  (boost::shared_ptr<U> origin,
   value_type scalar)
    : detail::AutopromoteTrait<U>::T_type
      (origin->inputSize (),
       origin->outputSize (),
       (boost::format ("%1% * %2%")
	% scalar
	% origin->getName ()).str ()),
      origin_ (origin),
      scalar_ (scalar)
  {}

  template <typename U>
  Scalar<U>::~Scalar ()
  {}

  template <typename U>
  void
  Scalar<U>::impl_compute
  (result_t& result, const argument_t& x)
    const
  {
    origin_->operator () (result, x);
    result *= scalar_;
  }

  template <typename U>
  void
  Scalar<U>::impl_gradient (gradient_t& gradient,
			 const argument_t& argument,
			 size_type functionId)
    const
  {
    origin_->gradient (gradient, argument, functionId);
    gradient *= scalar_;
  }

  template <typename U>
  void
  Scalar<U>::impl_jacobian (jacobian_t& jacobian,
			 const argument_t& argument)
    const
  {
    origin_->jacobian (jacobian, argument);
    jacobian *= scalar_;
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_SCALAR_HXX
