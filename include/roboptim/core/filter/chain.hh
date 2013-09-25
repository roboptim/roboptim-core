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
# include <vector>
# include <boost/make_shared.hpp>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \brief Chain two RobOptim functions.
  ///
  /// left(right(x))
  template <typename U, typename V>
  class Chain : public detail::PromoteTrait<U, V>::T_promote
  {
  public:
    typedef typename detail::PromoteTrait<U, V>::T_promote parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Chain> ChainShPtr_t;

    explicit Chain (boost::shared_ptr<U> left, boost::shared_ptr<V> right) throw ();
    ~Chain () throw ();

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
      const throw ();

    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			size_type functionId = 0)
      const throw ();
    void impl_jacobian (jacobian_t& jacobian,
			const argument_t& arg)
      const throw ();
  private:
    boost::shared_ptr<U> left_;
    boost::shared_ptr<V> right_;

    mutable result_t result_;
    mutable gradient_t gradientLeft_;
    mutable gradient_t gradientRight_;
    mutable jacobian_t jacobianLeft_;
    mutable jacobian_t jacobianRight_;
  };

  template <typename U, typename V>
  boost::shared_ptr<Chain<U, V> >
  chain (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
  {
    return boost::make_shared<Chain<U, V> > (left, right);
  }

} // end of namespace roboptim.

# include <roboptim/core/filter/chain.hxx>
#endif //! ROBOPTIM_CORE_FILTER_CHAIN_HH
