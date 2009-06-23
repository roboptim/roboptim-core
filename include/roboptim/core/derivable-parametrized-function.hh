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

#ifndef ROBOPTIM_TRAJECTORY_DERIVABLE_PARAMETRIZABLE_HH
# define ROBOPTIM_TRAJECTORY_DERIVABLE_PARAMETRIZABLE_HH
# include <utility>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/parametrized-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Parametrized function with parameter derivative available.
  ///
  /// Depending on inner function type, this class allows computation
  /// of parameter derivative or combined parameter/function derivative.
  ///
  /// \tparam F inner function type.
  template <typename F>
  class DerivableParametrizedFunction : public ParametrizedFunction<F>
  {
  public:
    /// \brief Import value type.
    typedef typename F::value_type value_type;
    /// \brief Import size type.
    typedef typename F::size_type size_type;
    /// \brief Import vector type.
    typedef typename F::vector_t vector_t;
    /// \brief Import matrix type.
    typedef typename F::matrix_t matrix_t;
    /// \brief Import  result type.
    typedef F result_t;
    /// \brief Import argument type.
    typedef typename F::argument_t argument_t;
    /// \brief Import gradient type.
    typedef typename F::vector_t gradient_t;
    /// \brief Import jacobian type.
    typedef typename F::matrix_t jacobian_t;

    /// \brief Import jacobian size type (pair of values).
    typedef typename F::jacobianSize_t jacobianSize_t;


    /// \brief Return the gradient size.
    ///
    /// Gradient size is equals to the input size.
    size_t gradientSize () const throw ()
    {
      return this->inputSize ();
    }

    /// \brief Return the jacobian size as a pair.
    ///
    /// Gradient size is equals to (output size, input size).
    jacobianSize_t jacobianSize () const throw ()
    {
      return std::make_pair (this->inputSize (),
			     this->functionOutputSize ());
    }

    /// \brief Check if the gradient is valid (check size).
    /// \param gradient checked gradient
    /// \return true if valid, false if not
    bool isValidGradient (const gradient_t& gradient) const throw ()
    {
      return gradient.size () == this->gradientSize ();
    }

    /// \brief Check if the jacobian is valid (check sizes).
    ///
    /// \param jacobian checked jacobian
    /// \return true if valid, false if not
    bool isValidJacobian (const jacobian_t& jacobian) const throw ()
    {
      return jacobian.size1 () == this->jacobianSize ().first
	&& jacobian.size2 () == this->jacobianSize ().second;
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
    /// Program will abort if the jacobian size is wrong before
    /// or after the jacobian computation.
    /// \param jacobian jacobian will be stored in this argument
    /// \param argument inner function point argument value
    void jacobian (jacobian_t& jacobian, const argument_t& argument) const throw ()
    {
      assert (argument.size () == this->inputSize ());
      assert (this->isValidJacobian (jacobian));
      this->impl_jacobian (jacobian, argument);
      assert (this->isValidJacobian (jacobian));
    }

    /// \brief Computes the gradient.
    ///
    /// \param argument inner function argument value
    /// \param functionId function id in split representation
    /// \return gradient vector
    gradient_t gradient (const argument_t& argument,
			 size_type functionId = 0) const throw ()
    {
      gradient_t gradient (gradientSize ());
      gradient.clear ();
      this->gradient (gradient, argument, functionId);
      return gradient;
    }

    /// \brief Computes the gradient.
    ///
    /// Program will abort if the gradient size is wrong before
    /// or after the gradient computation.
    /// \param gradient gradient will be stored in this argument
    /// \param argument inner function point argument value
    /// \param functionId function id in split representation
    /// \return gradient vector
    void gradient (gradient_t& gradient,
		   const argument_t& argument,
		   size_type functionId = 0) const throw ()
    {
      assert (argument.size () == this->inputSize ());
      assert (this->isValidGradient (gradient));
      this->impl_gradient (gradient, argument, functionId);
      assert (this->isValidGradient (gradient));
    }

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const throw ()
    {
      return o << "Derivable parametrized function";
    }

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize parameter size
    /// \param functionInputSize inner function argument size
    /// \param functionOutputSize inner function result size
    DerivableParametrizedFunction (size_type inputSize,
			  size_type functionInputSize,
			  size_type functionOutputSize) throw ()
      : ParametrizedFunction<F> (inputSize,
				 functionInputSize,
				 functionOutputSize)
    {
    }

    /// \brief Jacobian evaluation.
    ///
    /// Computes the jacobian, can be overridden by concrete classes.
    /// The default behavior is to compute the jacobian from the gradient.
    /// \warning Do not call this function directly, call #jacobian instead.
    /// \param jacobian jacobian will be store in this argument
    /// \param arg point where the jacobian will be computed
    virtual void impl_jacobian (jacobian_t& jacobian, const argument_t& arg)
      const throw ()
    {
      for (unsigned i = 0; i < this->functionOutputSize (); ++i)
	{
	  gradient_t grad = this->gradient (arg, i);
	  for (unsigned j = 0; j < this->inputSize (); ++j)
	    jacobian (i, j) = grad[j];
      }
    }

    /// \brief Gradient evaluation.
    ///
    /// Compute the gradient, has to be implemented in concrete classes.
    /// The gradient is computed for a specific sub-function which id
    /// is passed through the functionId argument.
    /// \warning Do not call this function directly, call #gradient instead.
    /// \param gradient gradient will be store in this argument
    /// \param argument inner function point argument value
    /// \param functionId evaluated function id in the split representation
    virtual void impl_gradient (gradient_t& gradient,
				const argument_t& argument,
				size_type functionId = 0)
      const throw () = 0;
  };

  /// @}

} // end of namespace roboptim.

#endif //! ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
