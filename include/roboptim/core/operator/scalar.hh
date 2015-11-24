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

#ifndef ROBOPTIM_CORE_OPERATOR_SCALAR_HH
# define ROBOPTIM_CORE_OPERATOR_SCALAR_HH

# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/twice-differentiable-function.hh>


namespace roboptim
{
  /// \addtogroup roboptim_operator
  /// @{

  /// \brief Multiply by a constant scalar value.
  /// \tparam U input function type.
  template <typename U>
  class Scalar : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    typedef typename parentType_t::traits_t traits_t;
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
      (GenericTwiceDifferentiableFunction<traits_t>);

    typedef boost::shared_ptr<Scalar> ScalarShPtr_t;

    /// \brief Create a scalar operator.
    /// \param fct input function.
    /// \param scalar scalar value.
    explicit Scalar (boost::shared_ptr<U> fct,
		     value_type scalar);
    ~Scalar ();

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

    void impl_hessian (hessian_ref hessian,
                       const_argument_ref x,
                       size_type functionId = 0) const;

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

  private:
    boost::shared_ptr<U> origin_;

    value_type scalar_;
  };

  template <typename U>
  boost::shared_ptr<Scalar<U> >
  scalar (boost::shared_ptr<U> origin,
	  typename Scalar<U>::size_type start = 0,
	  typename Scalar<U>::size_type size = 1)
  {
    return boost::make_shared<Scalar<U> > (origin, start, size);
  }

  template <typename U>
  boost::shared_ptr<Scalar<U> >
  operator* (typename Scalar<U>::value_type scalar,
	     boost::shared_ptr<U> origin)
  {
    return boost::make_shared<Scalar<U> > (origin, scalar);
  }

  template <typename U>
  boost::shared_ptr<Scalar<U> >
  operator* (boost::shared_ptr<U> origin,
	     typename Scalar<U>::value_type scalar)
  {
    return boost::make_shared<Scalar<U> > (origin, scalar);
  }

  template <typename U>
  boost::shared_ptr<U>
  operator+ (boost::shared_ptr<U> origin)
  {
    return origin;
  }

  template <typename U>
  boost::shared_ptr<Scalar<U> >
  operator- (boost::shared_ptr<U> origin)
  {
    return boost::make_shared<Scalar<U> > (origin, -1.);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/operator/scalar.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_SCALAR_HH
