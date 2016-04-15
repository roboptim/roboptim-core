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

#ifndef ROBOPTIM_CORE_OPERATOR_CONCATENATE_HH
# define ROBOPTIM_CORE_OPERATOR_CONCATENATE_HH
# include <stdexcept>
# include <vector>

# include <boost/shared_ptr.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_same.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  namespace detail
  {
    template <typename T>
    struct ConcatenateTypes;
  } // end of namespace detail

  /// \addtogroup roboptim_operator
  /// @{

  /// \brief Concatenate the output of two functions.
  /// \tparam U input functions type.
  template <typename U>
  class Concatenate : public detail::AutopromoteTrait<U>::T_type
  {
  public:
    typedef typename detail::AutopromoteTrait<U>::T_type parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    /// \brief Traits type.
    typedef typename parentType_t::traits_t traits_t;

    typedef boost::shared_ptr<Concatenate> ConcatenateShPtr_t;

    explicit Concatenate (boost::shared_ptr<U> left,
			  boost::shared_ptr<U> right);
    ~Concatenate ();

    const boost::shared_ptr<U>& left () const
    {
      return left_;
    }

    boost::shared_ptr<U>& left ()
    {
      return left_;
    }

    const boost::shared_ptr<U>& right () const
    {
      return right_;
    }

    boost::shared_ptr<U>& right ()
    {
      return right_;
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

    /// \internal
    /// The two following function definitions should be put in the .hxx file.
    /// However, msvc compilers (at least up to Visual Sutdio 2015 Update 1) 
    /// fails in this case: it tries to match the definitions with the 
    /// declarations but since it does so before any substitution of any 
    ///template parameters, it can't distiguish between the two declarations.

    template <typename T>
    void concatenateJacobian(jacobian_ref jacobian,
      const_argument_ref x,
      typename detail::ConcatenateTypes<T>::isDense_t::type* = 0)
      const
    {
      left_->jacobian(jacobianLeft_, x);
      right_->jacobian(jacobianRight_, x);
      jacobian.middleRows(0, left_->outputSize()) =
        jacobianLeft_;
      jacobian.middleRows(left_->outputSize(), right_->outputSize()) =
        jacobianRight_;
    }

    template <typename T>
    void concatenateJacobian(jacobian_ref jacobian,
      const_argument_ref x,
      typename detail::ConcatenateTypes<T>::isNotDense_t::type* = 0)
      const
    {
      left_->jacobian(jacobianLeft_, x);
      right_->jacobian(jacobianRight_, x);

      copySparseBlock(jacobian, jacobianLeft_, 0, 0);
      copySparseBlock(jacobian, jacobianRight_, left_->outputSize(), 0);
    }

  private:
    boost::shared_ptr<U> left_;
    boost::shared_ptr<U> right_;

    mutable result_t resultLeft_;
    mutable result_t resultRight_;
    mutable jacobian_t jacobianLeft_;
    mutable jacobian_t jacobianRight_;
  };

  template <typename U, typename V>
  boost::shared_ptr<Concatenate<typename detail::PromoteTrait<U, V>::T_promote > >
  concatenate (boost::shared_ptr<U> left,
	       boost::shared_ptr<V> right)
  {
    typedef typename detail::PromoteTrait<U, V>::T_promote result_t;

    return boost::make_shared<Concatenate<result_t> > (left, right);
  }

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/operator/concatenate.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_CONCATENATE_HH
