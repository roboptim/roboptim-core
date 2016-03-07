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

#ifndef ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION_HH
# define ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION_HH
# include <cstring>
# include <limits>
# include <stdexcept>
# include <utility>

# include <log4cxx/logger.h>

# include <roboptim/core/fwd.hh>

# include <roboptim/core/function.hh>

# define ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS(PARENT)	\
  ROBOPTIM_FUNCTION_FWD_TYPEDEFS (PARENT);			\
  ROBOPTIM_GENERATE_FWD_REFS (gradient);			\
  ROBOPTIM_GENERATE_FWD_REFS (jacobian)

# define ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_(PARENT)	\
  ROBOPTIM_FUNCTION_FWD_TYPEDEFS_ (PARENT);			\
  ROBOPTIM_GENERATE_FWD_REFS_ (gradient);			\
  ROBOPTIM_GENERATE_FWD_REFS_ (jacobian)

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Define an abstract derivable function (\f$C^1\f$).
  ///
  /// A derivable function which provides a way to compute its
  /// gradient/jacobian.
  ///
  /// \f[ f : x \rightarrow f(x) \f]
  /// \f$x \in \mathbb{R}^n\f$, \f$f(x) \in \mathbb{R}^m\f$ where
  /// \f$n\f$ is the input size and \f$m\f$ is the output size.
  ///
  /// Gradient computation is done through the #impl_gradient method
  /// that has to implemented by the concrete class inheriting this
  /// class.
  ///
  /// Jacobian computation is automatically done by concatenating
  /// gradients together, however this naive implementation can be
  /// overridden by the concrete class.
  ///
  ///
  /// The gradient of a \f$\mathbb{R}^n \rightarrow \mathbb{R}^m\f$
  /// function where \f$n > 1\f$ and \f$m > 1\f$ is a matrix.
  /// As this representation is costly, RobOptim considers
  /// these functions as \f$m\f$ \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$
  /// functions. Through that mechanism, gradients are always vectors
  /// and jacobian are always matrices.
  /// When the gradient or the jacobian has to be computed, one has to
  /// precise which of the \f$m\f$ functions should be considered.
  ///
  /// If \f$m = 1\f$, then the function id must always be 0 and can be safely
  /// ignored in the gradient/jacobian computation.
  /// The class provides a default value for the function id so that
  /// these functions do not have to explicitly set the function id.
  template <typename T>
  class GenericDifferentiableFunction : public GenericFunction<T>
  {
  public:
    ROBOPTIM_FUNCTION_FWD_TYPEDEFS_ (GenericFunction<T>);
    ROBOPTIM_ADD_FLAG(ROBOPTIM_IS_DIFFERENTIABLE);

    /// \brief Gradient type.
    ROBOPTIM_GENERATE_TRAITS_REFS_(gradient);
    /// \brief Jacobian type.
    ROBOPTIM_GENERATE_TRAITS_REFS_(jacobian);

    /// \brief Jacobian size type (pair of values).
    typedef std::pair<size_type, size_type> jacobianSize_t;


    /// \brief Return the gradient size.
    ///
    /// Gradient size is equals to the input size.
    size_type gradientSize () const
    {
      return this->inputSize ();
    }

    /// \brief Return the jacobian size as a pair.
    ///
    /// Gradient size is equals to (output size, input size).
    jacobianSize_t jacobianSize () const
    {
      return std::make_pair (this->outputSize (), this->inputSize ());
    }

    /// \brief Check if the gradient is valid (check size).
    /// \param gradient checked gradient
    /// \return true if valid, false if not
    bool isValidGradient (const_gradient_ref gradient) const
    {
      return gradient.size () == gradientSize ();
    }

    /// \brief Check if the jacobian is valid (check sizes).
    ///
    /// \param jacobian checked jacobian
    /// \return true if valid, false if not
    bool isValidJacobian (const_jacobian_ref jacobian) const
    {
      return jacobian.rows () == jacobianSize ().first
	&& jacobian.cols () == jacobianSize ().second;
    }

    /// \brief Computes the jacobian.
    ///
    /// \param argument point at which the jacobian will be computed
    /// \return jacobian matrix
    jacobian_t jacobian (const_argument_ref argument) const
    {
      jacobian_t jacobian (jacobianSize ().first, jacobianSize ().second);
      jacobian.setZero ();
      this->jacobian (jacobian, argument);
      return jacobian;
    }

    /// \brief Computes the jacobian.
    ///
    /// Program will abort if the jacobian size is wrong before
    /// or after the jacobian computation.
    /// \param jacobian jacobian will be stored in this argument
    /// \param argument point at which the jacobian will be computed
    void jacobian (jacobian_ref jacobian, const_argument_ref argument)
      const
    {
      LOG4CXX_TRACE (this->logger,
		     "Evaluating jacobian at point: " << argument);
      assert (argument.size () == this->inputSize ());
      assert (isValidJacobian (jacobian));

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      bool cur_malloc_allowed = is_malloc_allowed ();
      set_is_malloc_allowed (false);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      this->impl_jacobian (jacobian, argument);

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      assert (isValidJacobian (jacobian));
    }

    /// \brief Computes the gradient.
    ///
    /// \param argument point at which the gradient will be computed
    /// \param functionId function id in split representation
    /// \return gradient vector
    gradient_t gradient (const_argument_ref argument,
			 size_type functionId = 0) const
    {
      assert (functionId < this->outputSize ());
      gradient_t gradient (gradientSize ());
      gradient.setZero ();
      this->gradient (gradient, argument, functionId);
      return gradient;
    }

    /// \brief Computes the gradient.
    ///
    /// Program will abort if the gradient size is wrong before
    /// or after the gradient computation.
    /// \param gradient gradient will be stored in this argument
    /// \param argument point at which the gradient will be computed
    /// \param functionId function id in split representation
    /// \return gradient vector
    void gradient (gradient_ref gradient,
		   const_argument_ref argument,
		   size_type functionId = 0) const
    {
      LOG4CXX_TRACE (this->logger,
		     "Evaluating gradient at point: "
		     << argument
		     << " (function id: " << functionId << ")");
      assert (functionId < this->outputSize ());
      assert (argument.size () == this->inputSize ());
      assert (isValidGradient (gradient));

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      bool cur_malloc_allowed = is_malloc_allowed ();
      set_is_malloc_allowed (false);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      this->impl_gradient (gradient, argument, functionId);

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      assert (isValidGradient (gradient));
    }

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize input size (argument size)
    /// \param outputSize output size (result size)
    /// \param name function's name
    GenericDifferentiableFunction (size_type inputSize,
				   size_type outputSize = 1,
				   std::string name = std::string ());

    /// \brief Jacobian evaluation.
    ///
    /// Computes the jacobian, can be overridden by concrete classes.
    /// The default behavior is to compute the jacobian from the gradient.
    /// \warning Do not call this function directly, call #jacobian instead.
    /// \param jacobian jacobian will be store in this argument
    /// \param arg point where the jacobian will be computed
    virtual void impl_jacobian (jacobian_ref jacobian, const_argument_ref arg)
      const;

    /// \brief Gradient evaluation.
    ///
    /// Compute the gradient, has to be implemented in concrete classes.
    /// The gradient is computed for a specific sub-function whose id
    /// is passed through the functionId argument.
    /// \warning Do not call this function directly, call #gradient instead.
    /// \param gradient gradient will be stored in this vector
    /// \param argument point where the gradient will be computed
    /// \param functionId evaluated function id in the split representation
    virtual void impl_gradient (gradient_ref gradient,
				const_argument_ref argument,
				size_type functionId = 0)
      const = 0;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/differentiable-function.hxx>
#endif //! ROBOPTIM_CORE_DIFFERENTIABLE_FUNCTION_HH
