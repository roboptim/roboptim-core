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

#ifndef ROBOPTIM_CORE_OPERATOR_BIND_HH
# define ROBOPTIM_CORE_OPERATOR_BIND_HH
# include <stdexcept>
# include <vector>
# include <boost/make_shared.hpp>
# include <boost/optional.hpp>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \addtogroup roboptim_operator
  /// @{

  /// \brief Bind some function input to a constant value.
  ///
  /// This allows to reduce any function input space by setting some
  /// inputs to particular values.
  ///
  /// \tparam U input function type.
  template <typename U>
  class Bind : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Bind> BindShPtr_t;
    typedef std::vector<boost::optional<value_type> > boundValues_t;

    explicit Bind (boost::shared_ptr<U> origin,
		   const boundValues_t& selector);
    ~Bind ();

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

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

  private:
    boost::shared_ptr<U> origin_;
    boundValues_t boundValues_;
    mutable vector_t x_;
    mutable gradient_t gradient_;
    mutable jacobian_t jacobian_;
  };

  template <typename U>
  boost::shared_ptr<Bind<U> >
  bind (boost::shared_ptr<U> origin,
	const typename Bind<U>::boundValues_t& boundValues)
  {
    return boost::make_shared<Bind<U> > (origin, boundValues);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/operator/bind.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_BIND_HH
