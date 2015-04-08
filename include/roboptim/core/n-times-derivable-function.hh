// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# define ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <boost/static_assert.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/twice-differentiable-function.hh>

# define ROBOPTIM_NTIMES_DERIVABLE_FUNCTION_FWD_TYPEDEFS(PARENT)	\
  ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS(PARENT);		\
  ROBOPTIM_GENERATE_FWD_REFS (derivative)

# define ROBOPTIM_NTIMES_DERIVABLE_FUNCTION_FWD_TYPEDEFS_(PARENT)	\
  ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_(PARENT);		\
  ROBOPTIM_GENERATE_FWD_REFS_ (derivative)

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  template <unsigned dorder>
  class NTimesDerivableFunction;

  /// \brief Explicit specialization for the stop case of NTimesDerivable class.
  ///
  /// This specialization defines the interface of a
  /// ``n times derivable function'' and implements generic methods required by
  /// upper classes using this class specific interface.
  template <>
  class NTimesDerivableFunction<2> : public TwiceDifferentiableFunction
  {
  public:

    /// \brief Parent type.
    typedef TwiceDifferentiableFunction parent_t;

    /// \brief Traits type.
    typedef parent_t::traits_t traits_t;

    /// \brief Derivative type.
    ///
    /// Derivatives are column vectors.
    ROBOPTIM_GENERATE_TRAITS_REFS_T(derivative,traits_t);

    /// \brief Function derivability order. One static const variable per class
    /// in inheritance structure.
    static const size_type derivabilityOrder = 2;

    /// \brief Returns the maximum derivability order (relevant for N>2 only)
    virtual size_type derivabilityOrderMax () const
    {
      return 2;
    }

    virtual ~NTimesDerivableFunction () {}


    /// \brief Return the size of the derivative vector.
    /// \return derivative vector size
    size_type derivativeSize () const
    {
      return outputSize ();
    }

    /// \brief Check if a derivative is valid (check sizes).
    /// \param derivative derivative vector to be checked
    /// \return true if valid, false if not
    bool isValidDerivative (const_derivative_ref derivative) const
    {
      return derivative.size () == this->derivativeSize ();
    }


    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param argument point at which the function will be evaluated
    /// \return computed result
    result_t operator () (value_type argument) const
    {
      result_t result (outputSize ());
      result.setZero ();
      (*this) (result, argument);
      return result;
    }


    /// \brief Evaluate the function at a specified point.
    ///
    /// The program will abort if the argument does not have the
    /// expected size.
    /// \param result result will be stored in this vector
    /// \param argument point at which the function will be evaluated
    /// \return computed result
    void operator () (result_ref result, value_type argument) const
    {
      assert (isValidResult (result));
      this->impl_compute (result, argument);
      assert (isValidResult (result));
    }


    /// \brief Compute the derivative of the function.
    /// Derivative is computed for a certain order, at a given point.
    /// \param argument point at which the derivative will be computed
    /// \param order derivative order (if 0 then function is evaluated)
    /// \return derivative vector
    derivative_t derivative (value_type argument, size_type order = 1) const
    {
      derivative_t derivative (derivativeSize ());
      derivative.setZero ();
      this->derivative (derivative, argument, order);
      return derivative;
    }


    /// \brief Compute the derivative of the function.
    /// Derivative is computed for a certain order, at a given point.
    /// \param derivative derivative will be stored in this vector
    /// \param argument point at which the derivative will be computed
    /// \param order derivative order (if 0 then function is evaluated)
    void derivative (derivative_ref derivative,
		     value_type argument,
		     size_type order = 1) const
    {
      assert (order <= derivabilityOrderMax ()
	      && isValidDerivative (derivative));
      this->impl_derivative (derivative, argument, order);
      assert (isValidDerivative (derivative));
    }


    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const
    {
      o << derivabilityOrderMax () << "-time differentiable function.";
      return o;
    }

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param outputSize output size (result size)
    /// \param name function's name
    NTimesDerivableFunction (size_type outputSize = 1,
			     std::string name = std::string ())
      : TwiceDifferentiableFunction (1, outputSize, name)
    {}

    /// \brief Function evaluation.
    ///
    /// Implement generic function evaluation, as required by
    /// Function, using this class evaluation method (using a double
    /// instead of a vector).
    ///
    /// \warning Do not call this function directly, call
    /// #operator()(result_ref, const_argument_ref) const
    /// instead.
    ///
    /// \param result result will be stored in this vector
    /// \param argument point at which the function will be evaluated
    void impl_compute (result_ref result, const_argument_ref argument)
      const
    {
      (*this) (result, argument[0]);
    }

    /// \brief Function evaluation.
    ///
    /// Evaluate the function, has to be implemented in concrete
    /// classes.  \warning Do not call this function directly, call
    /// #operator()(double) const instead.  \param result
    /// result will be stored in this vector \param t point at which
    /// the function will be evaluated
    virtual void impl_compute (result_ref result, value_type t) const = 0;

    /// \brief Gradient evaluation.
    ///
    /// Implement the gradient computation, as required by DerivableFunction.
    /// The gradient is computed for a specific sub-function which id
    /// is passed through the functionId argument.
    /// \warning Do not call this function directly, call #gradient
    /// or #derivative instead.
    /// \param gradient gradient will be store in this argument
    /// \param argument point where the gradient will be computed
    /// \param functionId evaluated function id in the split representation
    void impl_gradient (gradient_ref gradient,
			const_argument_ref argument,
			size_type functionId = 0) const
    {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      Eigen::internal::set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      derivative_t derivative (derivativeSize ());
      derivative.setZero ();

      this->derivative (derivative, argument[0], 1);
      gradient[0] = derivative[functionId];
    }


    /// \brief Derivative evaluation.
    ///
    /// Compute the derivative, has to be implemented in concrete classes.
    /// \warning Do not call this function directly, call #derivative instead.
    /// \param derivative derivative will be store in this argument
    /// \param argument point where the gradient will be computed
    /// \param order derivative order (if 0 evaluates the function)
    virtual void impl_derivative (derivative_ref derivative,
				  value_type argument,
				  size_type order = 1) const = 0;

    /// \brief Hessian evaluation.
    ///
    /// Implement the hessian computation, as required by the
    /// TwiceDerivableFunction class using the #derivative method.
    /// The hessian is computed for a specific sub-function which id is
    /// passed through the functionId argument.
    /// \warning Do not call this function directly, call #hessian instead.
    /// \param hessian hessian will be stored here
    /// \param argument point where the hessian will be computed
    /// \param functionId evaluated function id in the split representation
    void impl_hessian (hessian_ref hessian,
		       const_argument_ref argument,
		       size_type functionId = 0) const
    {
# ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      Eigen::internal::set_is_malloc_allowed (true);
# endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      assert (functionId == 0);

      derivative_t derivative (derivativeSize ());
      derivative.setZero ();

      this->derivative (derivative, argument[0], 2);
      hessian (0, 0) = derivative[functionId];
    }
  };

  /// \brief Define a \f$\mathbb{R} \rightarrow \mathbb{R}^m\f$ function,
  /// derivable n times (\f$n \geq 2\f$).
  template <unsigned DerivabilityOrder>
  class NTimesDerivableFunction
    : public NTimesDerivableFunction<DerivabilityOrder - 1>
  {
    BOOST_STATIC_ASSERT(DerivabilityOrder > 2);
  public:
    /// \brief Import size type from function.
    typedef Function::size_type size_type;

    /// \brief Function derivability order.
    static const size_type derivabilityOrder = DerivabilityOrder;
    /// \brief Returns the maximum derivability order.
    virtual size_type derivabilityOrderMax () const
    {
      return DerivabilityOrder;
    }

    virtual ~NTimesDerivableFunction ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param outputSize output size (result size)
    /// \param name function name
    NTimesDerivableFunction (size_type outputSize = 1,
			     std::string name = std::string ())
      : NTimesDerivableFunction<DerivabilityOrder - 1> (outputSize, name)
    {}
  };

  /// @}

} // end of namespace roboptim.

# include <roboptim/core/n-times-derivable-function.hxx>
#endif //! ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
