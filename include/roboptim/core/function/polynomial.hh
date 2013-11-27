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
# include <stdexcept>

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
    Polynomial(vector_t coefficients) throw (std::runtime_error)
      : GenericTwiceDifferentiableFunction<T>
	(1, 1, "polynomial"),
        coeffs_(coefficients),
        dCoeffs_(),
        dDCoeffs_()
    {
      if (coeffs_.size () < 1)
        throw std::runtime_error
          ("Bad coefficients vector size (must be at least 1 but vector is of null size)");
      if (coeffs_.size() == 1)
      {
        dCoeffs_.resize(1);
        dCoeffs_[0] = 0;
        dDCoeffs_.resize(1);
        dDCoeffs_[0] = 0;
      }
      else if (coeffs_.size() == 2)
      {
        dCoeffs_.resize(1);
        dCoeffs_[0] = coeffs_[1];
        dDCoeffs_.resize(1);
        dDCoeffs_[0] = 0;
      }
      else
      {
        dCoeffs_.resize(coeffs_.size() - 1);
        dDCoeffs_.resize(coeffs_.size() - 2);
        for(size_type deg = 1; deg < coeffs_.size(); ++deg)
        {
          dCoeffs_[deg-1] = deg*coeffs_[deg];
        }
        for(size_type deg = 2; deg < coeffs_.size(); ++deg)
        {
          dDCoeffs_[deg-2] = deg*(deg-1)*coeffs_[deg];
        }
      }
    }

    ~Polynomial () throw ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const throw ()
    {
      o << "Polynomial function:\n";
      o << coeffs_[0];
      for(size_type deg = 1; deg < coeffs_.size(); ++deg)
      {
        o << "+" << coeffs_[deg] << "pow(x," << deg << ")";
      }
      o << "\nFirst derivative:\n";
      o << dCoeffs_[0];
      for(size_type deg = 1; deg < dCoeffs_.size(); ++deg)
      {
        o << "+" << dCoeffs_[deg] << "pow(x," << deg << ")";
      }
      o << "\nSecond derivative:\n";
      o << dDCoeffs_[0];
      for(size_type deg = 1; deg < dDCoeffs_.size(); ++deg)
      {
        o << "+" << dDCoeffs_[deg] << "pow(x," << deg << ")";
      }
      o << "\n";
      return o;
    }

  protected:
    void impl_compute (result_t& result, const argument_t& x) const throw ()
    {
      result[0] = applyPolynomial(coeffs_, x); 
    }

    void impl_gradient (gradient_t& gradient, const argument_t& x, size_type)
    const throw ();

    void impl_jacobian (jacobian_t& jacobian, const argument_t& x) const throw ();

    void impl_hessian
    (hessian_t& hessian, const argument_t& x, size_type) const throw ();

    double applyPolynomial
    (const vector_t& coeffs, const argument_t& x) const throw (); 

  private:
    vector_t coeffs_; 
    vector_t dCoeffs_;    //first order derivative coeff 
    vector_t dDCoeffs_;   //second order derivative coeff
  };

  template <typename T>
  double Polynomial<T>::applyPolynomial
    (const vector_t& coeffs, const argument_t& x) const throw ()
  {
    double result = 0;
    double acc = 1;     //Accumulator
    for (size_type degree = 0; degree < coeffs.size(); 
        ++degree)
    {
      result += coeffs[degree] * acc;
      acc *= x[0];
    }  
    return result;
  } 


  template <>
  void
  Polynomial<EigenMatrixSparse>::impl_gradient (gradient_t& gradient, const argument_t& x, size_type)
    const throw ()
  {
    gradient.insert (0) = applyPolynomial(coeffs_, x);
  }

  template <typename T>
  void
  Polynomial<T>::impl_gradient (gradient_t& gradient, const argument_t& x, size_type)
    const throw ()
  {
    gradient[0] = applyPolynomial(dCoeffs_, x);
  }

  template <>
  void
  Polynomial<EigenMatrixSparse>::impl_jacobian
  (jacobian_t& jacobian, const argument_t& x) const throw ()
  {
    jacobian.coeffRef (0, 0) = applyPolynomial(dCoeffs_, x);
  }
  template <typename T>
  void
  Polynomial<T>::impl_jacobian
  (jacobian_t& jacobian, const argument_t& x) const throw ()
  {
    jacobian (0, 0) = applyPolynomial(dCoeffs_, x); 
  }

  template <typename T>
  void
  Polynomial<T>::impl_hessian (hessian_t& hessian,
			const argument_t& x,
			size_type) const throw ()
  {
    hessian (0, 0) = applyPolynomial(dDCoeffs_, x);
  }

  template <>
  void
  Polynomial<EigenMatrixSparse>::impl_hessian (hessian_t& hessian,
					const argument_t& x,
					size_type) const throw ()
  {
    hessian.insert (0, 0) = applyPolynomial(dDCoeffs_, x);
  }

  /// Example shows constant function use.
  /// \example function-polynomial.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_POLYNOMIAL_HH
