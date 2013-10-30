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

#ifndef ROBOPTIM_CORE_FILTER_BIND_HH
# define ROBOPTIM_CORE_FILTER_BIND_HH
# include <stdexcept>
# include <vector>
# include <boost/optional.hpp>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \brief Bind some function input to a constant value.
  ///
  // This allows to reduce any function input space by setting some
  // inputs to particular values.
  template <typename U>
  class Bind : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Bind> BindShPtr_t;
    typedef std::vector<boost::optional<value_type> > boundValues_t;

    explicit Bind (boost::shared_ptr<U> origin,
		   const boundValues_t& selector)
      throw (std::exception);
    ~Bind () throw ();

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
    boundValues_t boundValues_;
    mutable argument_t x_;
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

} // end of namespace roboptim.

# include <roboptim/core/filter/bind.hxx>
#endif //! ROBOPTIM_CORE_FILTER_BIND_HH
