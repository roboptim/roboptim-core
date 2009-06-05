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

#ifndef ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
# define ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
# include <limits>
# include <utility>

# include <roboptim/core/function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Define a derivable function (\f$C^1\f$).
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
  class DerivableFunction : public Function
  {
  public:
    /// \brief Gradient type.
    typedef vector_t gradient_t;
    /// \brief Jacobian type.
    typedef matrix_t jacobian_t;

    /// \brief Jacobian size type (pair of values).
    typedef std::pair<value_type, value_type> jacobianSize_t;


    /// \brief Return the gradient size.
    ///
    /// Gradient size is equals to the input size.
    size_t gradientSize () const throw ()
    {
      return inputSize ();
    }

    /// \brief Return the jacobian size as a pair.
    ///
    /// Gradient size is equals to (output size, input size).
    jacobianSize_t jacobianSize () const throw ()
    {
      return std::make_pair (outputSize (), inputSize ());
    }

    /// \brief Check if the gradient is valid (check size).
    /// \param gradient checked gradient
    /// \return true if valid, false if not
    bool isValidGradient (const gradient_t& gradient) const throw ()
    {
      return gradient.size () == gradientSize ();
    }

    /// \brief Check if the jacobian is valid (check sizes).
    ///
    /// \param jacobian checked jacobian
    /// \return true if valid, false if not
    bool isValidJacobian (const jacobian_t& jacobian) const throw ()
    {
      return jacobian.size1 () == jacobianSize ().first
	&& jacobian.size2 () == jacobianSize ().second;
    }

    /// \brief Computes the jacobian.
    ///
    /// \param argument point at which the jacobian will be computed
    /// \return jacobian matrix
    jacobian_t jacobian (const argument_t& argument) const throw ()
    {
      jacobian_t jacobian (jacobianSize ().first, jacobianSize ().second);
      jacobian.clear ();
      this->jacobian (jacobian, argument);
      return jacobian;
    }

    /// \brief Computes the jacobian.
    ///
    /// \param jacobian jacobian will be stored in this argument
    /// \param argument point at which the jacobian will be computed
    void jacobian (jacobian_t& jacobian, const argument_t& argument) const throw ()
    {
      assert (isValidJacobian (jacobian));
      this->impl_jacobian (jacobian, argument);
      assert (isValidJacobian (jacobian));
    }

    /// \brief Computes the gradient.
    ///
    /// \param argument point at which the gradient will be computed
    /// \return gradient vector
    gradient_t gradient (const argument_t& argument,
			 int functionId = 0) const throw ()
    {
      gradient_t gradient (gradientSize ());
      gradient.clear ();
      this->gradient (gradient, argument);
      return gradient;
    }

    /// \brief Computes the gradient.
    ///
    /// \param gradient gradient will be stored in this argument
    /// \param argument point at which the gradient will be computed
    /// \return gradient vector
    void gradient (gradient_t& gradient,
		   const argument_t& argument,
		   int functionId = 0) const throw ()
    {
      assert (isValidGradient (gradient));
      this->impl_gradient (gradient, argument, functionId);
      assert (isValidGradient (gradient));
    }

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \param output stream
    virtual std::ostream& print (std::ostream& o) const throw ();

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize input size (argument size)
    /// \param outputSize output size (result size)
    DerivableFunction (size_type inputSize, size_type outputSize = 1) throw ();

    /// \brief Jacobian evaluation.
    ///
    /// Computes the jacobian, can be overridden by concrete classes.
    /// The default behavior is to compute the jacobian from the gradient.
    /// \warning Do not call this function directly, call #jacobian instead.
    virtual void impl_jacobian (jacobian_t&, const argument_t&)
      const throw ();

    /// \brief Gradient evaluation.
    ///
    /// Compute the gradient, has to be implemented in concrete classes.
    /// The gradient is computed for a specific sub-function which id
    /// is passed through the functionId argument.
    /// \warning Do not call this function directly, call #gradient instead.
    virtual void impl_gradient (gradient_t& gradient,
				const argument_t& argument,
				int functionId = 0)
      const throw () = 0;
  };

  /// @}

} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
