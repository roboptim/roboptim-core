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
    explicit Polynomial (const vector_t& coefficients);

    virtual ~Polynomial ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

  protected:
    void impl_compute (result_t& result, const argument_t& x) const;

    void impl_gradient (gradient_t& gradient, const argument_t& x, size_type)
      const;

    void impl_jacobian (jacobian_t& jacobian, const argument_t& x)
      const;

    void impl_hessian
    (hessian_t& hessian, const argument_t& x, size_type) const;

    /// \brief Implement Horner's method.
    value_type applyPolynomial
    (const vector_t& coeffs, const argument_t& x) const;

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

} // end of namespace roboptim

# include <roboptim/core/function/polynomial.hxx>

#endif //! ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH
