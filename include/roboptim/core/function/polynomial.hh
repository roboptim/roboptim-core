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

#ifndef ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH
# define ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH

# include <roboptim/core/fwd.hh>
# include <roboptim/core/twice-differentiable-function.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Polynomial function.
  ///
  /// Implement a polynomial function using the formula:
  /// \f[f(x) = a_0 + a_1 * x + a_2 * x^2 + a_3 * x^3 + ...\f]
  /// where polynomial coefficients \f$a_i\f$ are set when the class
  /// is instanciated
  template <typename T>
  class Polynomial : public GenericTwiceDifferentiableFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericTwiceDifferentiableFunction<T>);

    /// \brief Build a polynomial function
    ///
    /// \param coefficients polynomial coefficients
    /// given in increasing degree order
    /// \throw std::runtime_error
    explicit Polynomial (const_vector_ref coefficients);

    virtual ~Polynomial ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

  protected:
    void impl_compute (result_ref result, const_argument_ref x) const;

    void impl_gradient (gradient_ref gradient, const_argument_ref x, size_type)
      const;

    void impl_jacobian (jacobian_ref jacobian, const_argument_ref x)
      const;

    void impl_hessian
    (hessian_ref hessian, const_argument_ref x, size_type) const;

    /// \brief Implement Horner's method.
    value_type applyPolynomial
    (const_vector_ref coeffs, const_argument_ref x) const;

  public:
    /// \brief Get the type-checking flag
    virtual unsigned long flag() const
    {
      return flag_;
    }

    /// \brief Flag representing the Roboptim Function type
    static const unsigned long flag_;

  private:
    /// \brief Coefficients of the polynomial
    vector_t coeffs_;
    /// \brief Coefficients of the first derivative of the polynomial
    vector_t dCoeffs_;
    /// \brief Coefficients of the second derivative of the polynomial
    vector_t dDCoeffs_;
  };

  /// Example shows polynomial function use.
  /// \example function-polynomial.cc

  /// @}

  template <typename T>
  const unsigned long Polynomial<T>::flag_ = ROBOPTIM_IS_POLYNOMIAL|Polynomial<T>::parent_t::flag_;

} // end of namespace roboptim

# include <roboptim/core/function/polynomial.hxx>

#endif //! ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH
