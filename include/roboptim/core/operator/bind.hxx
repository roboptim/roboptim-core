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

#ifndef ROBOPTIM_CORE_OPERATOR_BIND_HXX
# define ROBOPTIM_CORE_OPERATOR_BIND_HXX

# include <stdexcept>
# include <boost/format.hpp>

# include <roboptim/core/indent.hh>

namespace roboptim
{
  template <typename U>
  Bind<U>::Bind
  (boost::shared_ptr<U> origin,
   const boundValues_t& boundValues)
    : detail::AutopromoteTrait<U>::T_type
      (static_cast<size_type>
       (std::count (boundValues.begin (), boundValues.end (),
		    boost::optional<value_type> ())),
       origin->outputSize (),
       (boost::format ("Bind(%1%)")
	% origin->getName ()).str ()),
      origin_ (origin),
      boundValues_ (boundValues),
      x_ (origin->inputSize ()),
      gradient_ (origin->inputSize ()),
      jacobian_ (origin->outputSize (),
		 origin->inputSize ())
  {
    gradient_.setZero ();
    jacobian_.setZero ();

    if (origin->inputSize () -
	static_cast<size_type> (boundValues.size ()) != 0)
      {
	boost::format fmt
	  ("function \"%s\" input size (%d)"
	   " and bounded value (%d) do not match");
	fmt
	  % origin->getName ()
	  % origin->inputSize () % boundValues.size ();
	throw std::runtime_error (fmt.str ().c_str ());
      }
  }

  template <typename U>
  Bind<U>::~Bind ()
  {}

  template <typename U>
  void
  Bind<U>::impl_compute
  (result_ref result, const_argument_ref x)
    const
  {
    size_type id = 0;
    for (std::size_t idx = 0; idx < boundValues_.size (); ++idx)
      if (boundValues_[idx])
	x_[static_cast<size_type> (idx)] = *(boundValues_[idx]);
      else
	x_[static_cast<size_type> (idx)] = x[id++];
    origin_->operator () (result, x_);
  }

  template <typename U>
  void
  Bind<U>::impl_gradient (gradient_ref gradient,
			  const_argument_ref argument,
			  size_type functionId)
    const
  {
    size_type id = 0;
    for (std::size_t idx = 0; idx < boundValues_.size (); ++idx)
      if (boundValues_[idx])
	x_[static_cast<size_type> (idx)] = *(boundValues_[idx]);
      else
	x_[static_cast<size_type> (idx)] = argument[id++];
    origin_->gradient (gradient_, x_, functionId);

    id = 0;
    for (std::size_t idx = 0; idx < boundValues_.size (); ++idx)
      if (!boundValues_[idx])
	gradient.coeffRef (id++) =
	  gradient_.coeffRef (static_cast<size_type> (idx));
  }

  template <typename U>
  void
  Bind<U>::impl_jacobian (jacobian_ref jacobian,
			  const_argument_ref argument)
    const
  {
    size_type id = 0;
    for (std::size_t idx = 0; idx < boundValues_.size (); ++idx)
      if (boundValues_[idx])
	x_[static_cast<size_type> (idx)] = *(boundValues_[idx]);
      else
	x_[static_cast<size_type> (idx)] = argument[id++];

    origin_->jacobian (jacobian_, x_);

    assert (jacobian_.rows () == jacobian.rows ());

    id = 0;
    for (size_type col = 0; col < jacobian_.cols (); ++col)
      if (!boundValues_[static_cast<std::size_t> (col)])
	{
	  for (size_type row = 0; row < jacobian_.rows (); ++row)
	    jacobian.coeffRef (row, id) = jacobian_.coeffRef (row, col);
	  ++id;
	}
  }


  template <typename U>
  std::ostream&
  Bind<U>::print (std::ostream& o) const
  {
    o << this->getName () << ":" << incindent;

    std::size_t n_val = boundValues_.size ()
      -  static_cast<std::size_t> (std::count (boundValues_.begin (),
                                               boundValues_.end (),
                                               boost::optional<value_type> ()));

    o << iendl << "Bound values: [" << n_val << "]{";

    std::size_t cnt = 0;
    for (std::size_t i = 0; i < boundValues_.size (); ++i)
      {
	if (boundValues_[i])
	  {
	    o << i << ": " << *boundValues_[i]
	      << ((cnt < n_val-1)? ", " : "");
	    cnt++;
	  }
      }

    o << "}" << decindent;

    return o;
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_OPERATOR_BIND_HXX
