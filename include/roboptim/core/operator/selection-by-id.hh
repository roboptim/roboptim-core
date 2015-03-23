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

#ifndef ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HH
# define ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HH
# include <stdexcept>
# include <vector>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \addtogroup roboptim_filter
  /// @{

  /// \brief Select part of a function.
  /// \tparam U input function type.
  template <typename U>
  class SelectionById : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<SelectionById> SelectionByIdShPtr_t;

    explicit SelectionById (boost::shared_ptr<U> origin,
			    std::vector<bool> selector);
    ~SelectionById ();

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
    std::vector<bool> selector_;

    mutable result_t result_;
    mutable gradient_t gradient_;
  };

  template <typename U>
  boost::shared_ptr<SelectionById<U> >
  selectionById (boost::shared_ptr<U> origin,
		 std::vector<bool> selector)
  {
    return boost::make_shared<SelectionById<U> > (origin, selector);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/operator/selection-by-id.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_SELECTION_BY_ID_HH
