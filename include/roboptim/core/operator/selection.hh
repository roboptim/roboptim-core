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

#ifndef ROBOPTIM_CORE_OPERATOR_SELECTION_HH
# define ROBOPTIM_CORE_OPERATOR_SELECTION_HH
# include <stdexcept>
# include <vector>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \addtogroup roboptim_operator
  /// @{

  /// \brief Select a block of a function's output.
  /// The selected block is a range given by a start and a size.
  /// \tparam U input function type.
  template <typename U>
  class Selection : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Selection> SelectionShPtr_t;

    /// \brief Create a selection given an input function and a block.
    /// \param fct input function.
    /// \param start start of the range.
    /// \param size size of the range.
    explicit Selection (boost::shared_ptr<U> fct,
			size_type start, size_type size);
    ~Selection ();

    const boost::shared_ptr<U>& origin () const
    {
      return origin_;
    }

    boost::shared_ptr<U>& origin ()
    {
      return origin_;
    }

    void impl_compute (result_ref result, const_argument_ref x)
      const;

    void impl_gradient (gradient_ref gradient,
			const_argument_ref argument,
			size_type functionId = 0)
      const;
    void impl_jacobian (jacobian_ref jacobian,
			const_argument_ref arg)
      const;
  private:
    boost::shared_ptr<U> origin_;

    size_type start_;
    size_type size_;

    mutable result_t result_;
    mutable gradient_t gradient_;
    mutable jacobian_t jacobian_;
  };

  template <typename U>
  boost::shared_ptr<Selection<U> >
  selection (boost::shared_ptr<U> origin,
	     typename Selection<U>::size_type start = 0,
	     typename Selection<U>::size_type size = 1)
  {
    return boost::make_shared<Selection<U> > (origin, start, size);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/operator/selection.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_SELECTION_HH
