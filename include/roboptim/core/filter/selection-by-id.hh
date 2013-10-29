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

#ifndef ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HH
# define ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HH
# include <stdexcept>
# include <vector>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \brief Select part of a function.
  template <typename U>
  class SelectionById : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<SelectionById> SelectionByIdShPtr_t;

    explicit SelectionById (boost::shared_ptr<U> origin,
			    std::vector<bool> selector)
      throw (std::runtime_error);
    ~SelectionById () throw ();

    const boost::shared_ptr<U>& origin () const
    {
      return origin_;
    }

    boost::shared_ptr<U>& origin ()
    {
      return origin_;
    }

    void impl_compute (result_t& result, const argument_t& x)
      const throw ();

    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			size_type functionId = 0)
      const throw ();
    void impl_jacobian (jacobian_t& jacobian,
			const argument_t& arg)
      const throw ();
  private:
    boost::shared_ptr<U> origin_;
    std::vector<bool> selector_;

    mutable result_t result_;
    mutable gradient_t gradient_;
    mutable jacobian_t jacobian_;
  };

  template <typename U>
  boost::shared_ptr<SelectionById<U> >
  selectionById (boost::shared_ptr<U> origin,
		 std::vector<bool> selector)
  {
    return boost::make_shared<SelectionById<U> > (origin, selector);
  }

} // end of namespace roboptim.

# include <roboptim/core/filter/selection-by-id.hxx>
#endif //! ROBOPTIM_CORE_FILTER_SELECTION_BY_ID_HH
