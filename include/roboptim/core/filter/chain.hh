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

#ifndef ROBOPTIM_CORE_FILTER_CHAIN_HH
# define ROBOPTIM_CORE_FILTER_CHAIN_HH
# include <stdexcept>
# include <vector>
# include <boost/make_shared.hpp>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \addtogroup roboptim_filter
  /// @{

  /// \brief Chain two RobOptim functions.
  ///
  ///
  /// Input: (size: right function input size)
  ///  x
  ///
  /// Output: x (size: left function output size)
  ///  left (right (x))
  ///
  /// (left (right (x)))' = left'(right(x)) * right'(x)
  ///
  /// \tparam U left input function type.
  /// \tparam V right input function type.
  template <typename U, typename V>
  class Chain : public detail::PromoteTrait<U, V>::T_promote
  {
  public:
    typedef typename detail::PromoteTrait<U, V>::T_promote parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Chain> ChainShPtr_t;

    /// \brief Chain filter constructor.
    ///
    /// \param left Left function
    /// \param right Right function
    explicit Chain (boost::shared_ptr<U> left, boost::shared_ptr<V> right);
    ~Chain ();

    const boost::shared_ptr<U>& left () const
    {
      return left_;
    }

    U& left ()
    {
      return left_;
    }

    const boost::shared_ptr<V>& right () const
    {
      return right_;
    }

    V& right ()
    {
      return right_;
    }

    void impl_compute (result_t& result, const argument_t& x)
      const;

    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			size_type functionId = 0)
      const;
    void impl_jacobian (jacobian_t& jacobian,
			const argument_t& arg)
      const;
  private:
    /// \brief Shared pointer to the left function.
    boost::shared_ptr<U> left_;
    /// \brief Shared pointer to the right function.
    boost::shared_ptr<V> right_;

    /// \name Buffers
    /// \{

    /// \brief Temporary buffer to store right function result.
    mutable result_t rightResult_;

    /// \brief Temporary buffer to store left function gradient.
    mutable gradient_t gradientLeft_;

    /// \brief Temporary buffer to store right function gradient.
    mutable gradient_t gradientRight_;

    /// \brief Temporary buffer to store left function jacobian.
    mutable jacobian_t jacobianLeft_;

    /// \brief Temporary buffer to store right function jacobian.
    mutable jacobian_t jacobianRight_;

    /// \}
  };

  /// \brief Chain two RobOptim functions.
  ///
  /// This will instantiate a Chain<U,V> RobOptim filter that will
  /// realize the underlying computations.
  template <typename U, typename V>
  boost::shared_ptr<Chain<U, V> >
  chain (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
  {
    return boost::make_shared<Chain<U, V> > (left, right);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/filter/chain.hxx>
#endif //! ROBOPTIM_CORE_FILTER_CHAIN_HH
