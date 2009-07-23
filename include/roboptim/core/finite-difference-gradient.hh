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

#ifndef ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# define ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# include <roboptim/core/fwd.hh>

# include <roboptim/core/derivable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Compute automatically a gradient with finite differences.
  ///
  /// Finite difference gradient is a method to approximate a function's
  /// gradient. It is particularly useful in RobOptim to avoid the need
  /// to compute the analytical gradient manually.
  ///
  /// This class takes a Function as its input and wraps it into a derivable
  /// function.
  ///
  /// The one dimensional formula is:
  /// \f[f'(x)\approx {f(x+\epsilon)-f(x)\over \epsilon}\f]
  /// where \f$\epsilon\f$ is a constant given when calling the class
  /// constructor.
  class FiniteDifferenceGradient : public DerivableFunction
  {
  public:
    /// \brief Instantiate a finite differences gradient.
    ///
    /// Instantiate a derivable function that will wraps a non
    /// derivable function and compute automatically its gradient
    /// using finite differences.
    /// \param f function that will e wrapped
    /// \param e epsilon used in finite difference computation
    FiniteDifferenceGradient (const Function& f, value_type e = 1e-8) throw ();
    ~FiniteDifferenceGradient () throw ();

  protected:
    void impl_compute (result_t&, const argument_t&) const throw ();
    void impl_gradient (gradient_t&, const argument_t& argument, size_type = 0)
      const throw ();

    /// \brief Reference to the wrapped function.
    const Function& adaptee_;

    //// \brief Epsilon used in finite differences computation.
    const value_type epsilon_;
  };

  /// \brief Check if a gradient is valid.
  ///
  /// Check if a gradient is valid by comparing the distance between its
  /// gradient and an automatically computed finite differences gradient.
  /// \param function function that will be checked
  /// \param functionId function id in split representation
  /// \param x point where the gradient will be evaluated
  /// \param threshold maximum tolerated error
  /// \return true if valid, false if not
  bool checkGradient (const DerivableFunction& function,
		      int functionId,
		      const Function::vector_t& x,
		      Function::value_type threshold = 1e-4) throw ();


  /// Example shows finite differences gradient use.
  /// \example finite-difference-gradient.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
