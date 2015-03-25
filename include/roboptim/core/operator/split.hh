// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_OPERATOR_SPLIT_HH
# define ROBOPTIM_CORE_OPERATOR_SPLIT_HH
# include <stdexcept>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/n-times-derivable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_operator
  /// @{

  /// \brief Select an element of a function's output.
  /// \tparam T input function type.
  template <typename T>
  class Split : public T
  {
  public:
    /// \brief Import traits type.
    typedef typename T::traits_t traits_t;

    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericTwiceDifferentiableFunction<traits_t>);

    /// \brief Import interval type.
    typedef typename DifferentiableFunction::interval_t interval_t;

    /// \brief Split operator constructor.
    /// \param fct input function.
    /// \param functionId index of the output to select.
    explicit Split (boost::shared_ptr<const T> fct,
		    size_type functionId);
    ~Split ();

  protected:
    virtual void impl_compute (result_ref result, const_argument_ref argument)
      const;


    virtual void impl_gradient (gradient_ref gradient,
				const_argument_ref argument,
				size_type functionId = 0)
      const;

    virtual void impl_hessian (hessian_ref hessian,
    			       const_argument_ref argument,
    			       size_type functionId = 0) const;

    virtual void impl_derivative (gradient_ref derivative,
    				  value_type argument,
    				  size_type order = 1) const;

  private:
    boost::shared_ptr<const T> function_;
    size_type functionId_;
    mutable result_t res_;
  };

  template <typename P, typename C>
  void addNonScalarConstraint
  (P& problem,
   boost::shared_ptr<C> constraint,
   std::vector<Function::interval_t> interval,
   std::vector<Function::value_type> scale
   = std::vector<Function::value_type> ());

  /// @}

} // end of namespace roboptim

# include <roboptim/core/operator/split.hxx>
#endif //! ROBOPTIM_CORE_OPERATOR_SPLIT_HH
