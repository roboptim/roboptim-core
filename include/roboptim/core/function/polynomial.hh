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
# include <stdexcept>

# include <boost/format.hpp>

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
    explicit Polynomial (const vector_t& coefficients)
      : GenericTwiceDifferentiableFunction<T>
	(1, 1, "polynomial"),
        coeffs_ (coefficients),
        dCoeffs_ (),
        dDCoeffs_ ()
    {
      typename vector_t::Index size = coeffs_.size ();

      if (size < 1)
        throw std::runtime_error
          ("Bad coefficients vector size"
	   " (must be at least 1 but vector is of null size)");

      if (size == 1)
	{
	  dCoeffs_.resize (1);
	  dCoeffs_[0] = 0;
	  dDCoeffs_.resize (1);
	  dDCoeffs_[0] = 0;
	  return;
	}

      if (size == 2)
	{
	  dCoeffs_.resize (1);
	  dCoeffs_[0] = coeffs_[1];
	  dDCoeffs_.resize (1);
	  dDCoeffs_[0] = 0;
	  return;
	}

      dCoeffs_.resize (coeffs_.size () - 1);
      dDCoeffs_.resize (coeffs_.size () - 2);
      for (typename vector_t::Index deg = 1; deg < coeffs_.size (); ++deg)
	dCoeffs_[deg - 1] = static_cast<value_type> (deg) * coeffs_[deg];
      for (typename vector_t::Index deg = 2; deg < coeffs_.size (); ++deg)
	dDCoeffs_[deg - 2] =
	  static_cast<value_type> (deg * (deg - 1)) * coeffs_[deg];
    }

    virtual ~Polynomial ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const
    {
      try
	{
	  boost::format fmt (" + %d pow(x, %d)");


	  o << "Polynomial function:" << incindent;

	  o << iendl << "Expression:" << incindent;
	  o << iendl << coeffs_[0];

	  for (typename vector_t::Index deg = 1; deg < coeffs_.size (); ++deg)
	    {
	      fmt % coeffs_[deg] % deg;
	      o << fmt.str ();
	    }
	  o << decindent;

	  o << iendl << "First derivative:" << incindent;
	  o << iendl << dCoeffs_[0];
	  for (typename vector_t::Index deg = 1; deg < dCoeffs_.size (); ++deg)
	    {
	      fmt % dCoeffs_[deg] % deg;
	      o << fmt.str ();
	    }
	  o << decindent;

	  o << iendl << "Second derivative:" << incindent;
	  o << iendl << dDCoeffs_[0];
	  for(typename vector_t::Index deg = 1; deg < dDCoeffs_.size (); ++deg)
	    {
	      fmt % dDCoeffs_[deg] % deg;
	      o << fmt.str ();
	    }
	  o << decindent;

	  o << decindent;

	  return o;
	}
      catch (std::exception& e)
	{
	  o << "Failed to display function, formatting error." << e.what ();
	  return o;
	}
    }

  protected:
    void impl_compute (result_t& result, const argument_t& x) const
    {
      result[0] = applyPolynomial (coeffs_, x);
    }

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

  template <typename T>
  typename Polynomial<T>::value_type Polynomial<T>::applyPolynomial
  (const vector_t& coeffs, const argument_t& x) const
  {
    // Accumulator
    value_type acc = 0;

    for (typename vector_t::Index degree = coeffs.size () - 1; degree >= 0;
	 --degree)
      acc = coeffs[degree] + acc * x[0];
    return acc;
  }

  template <typename T>
  void
  Polynomial<T>::impl_gradient (gradient_t& gradient,
				const argument_t& x, size_type)
    const
  {
    gradient.coeffRef (0) = applyPolynomial (dCoeffs_, x);
  }

  template <typename T>
  void
  Polynomial<T>::impl_jacobian
  (jacobian_t& jacobian, const argument_t& x) const
  {
    jacobian.coeffRef (0, 0) = applyPolynomial (dCoeffs_, x);
  }

  template <typename T>
  void
  Polynomial<T>::impl_hessian (hessian_t& hessian,
			       const argument_t& x,
			       size_type) const
  {
    hessian.coeffRef (0, 0) = applyPolynomial (dDCoeffs_, x);
  }

  /// Example shows polynomial function use.
  /// \example function-polynomial.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH
