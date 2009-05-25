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

/**
 * \brief Declaration of the DerivableFunction class.
 */

#ifndef ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
# define ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
# include <limits>
# include <utility>

# include <roboptim/core/function.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_function
     @{
  */
  /// Define a derivable function.
  class DerivableFunction : public Function
  {
  public:
    /// Gradient type.
    typedef vector_t gradient_t;
    /// Jacobian type.
    typedef matrix_t jacobian_t;

    /// Jacobian size type.
    typedef std::pair<value_type, value_type> jacobianSize_t;


    /// Return the gradient size.
    size_t gradientSize () const throw ()
    {
      return inputSize ();
    }

    /// Return the jacobian size as a pair.
    jacobianSize_t jacobianSize () const throw ()
    {
      return std::make_pair (outputSize (), inputSize ());
    }

    /// Check if the gradient is valid (check size).
    bool isValidGradient (const gradient_t& gradient) const throw ()
    {
      return gradient.size () == inputSize ();
    }

    /// Check if the jacobian is valid (check sizes).
    bool isValidJacobian (const jacobian_t& jacobian) const throw ()
    {
      return jacobian.size1 () == jacobianSize ().first
	&& jacobian.size2 () == jacobianSize ().second;
    }

    /// Computes the jacobian.
    jacobian_t jacobian (const argument_t& argument) const throw ()
    {
      jacobian_t jacobian (jacobianSize ().first, jacobianSize ().second);
      jacobian.clear ();
      this->jacobian (jacobian, argument);
      return jacobian;
    }

    /// Computes the jacobian.
    void jacobian (jacobian_t& jacobian, const argument_t& argument) const throw ()
    {
      assert (isValidJacobian (jacobian));
      this->impl_jacobian (jacobian, argument);
      assert (isValidJacobian (jacobian));
    }

    /// Computes the gradient.
    gradient_t gradient (const argument_t& argument,
			 int functionId = 0) const throw ()
    {
      gradient_t gradient (gradientSize ());
      gradient.clear ();
      this->gradient (gradient, argument);
      return gradient;
    }

    /// Computes the gradient.
    void gradient (gradient_t& gradient,
		   const argument_t& argument,
		   int functionId = 0) const throw ()
    {
      assert (isValidGradient (gradient));
      this->impl_gradient (gradient, argument, functionId);
      assert (isValidGradient (gradient));
    }

    /// Display the function.
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    /// Constructor.
    DerivableFunction (size_type n, size_type m = 1) throw ();

    /// Computes the jacobian, can be overridden by concrete classes.
    /// The default behavior is to compute the jacobian from the gradient.
    virtual void impl_jacobian (jacobian_t&, const argument_t&)
      const throw ();

    /// Compute the gradient, has to be implemented in concrete classes.
    /// The gradient is computed for a specific sub-function which id
    /// is passed through the functionId argument.
    virtual void impl_gradient (gradient_t& gradient,
				const argument_t& argument,
				int functionId = 0)
      const throw () = 0;
  };

/**
   @}
*/

} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_DERIVABLE_FUNCTION_HH
