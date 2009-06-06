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

#ifndef ROBOPTIM_CORE_NUMERIC_QUADRATIC_FUNCTION_HH
# define ROBOPTIM_CORE_NUMERIC_QUADRATIC_FUNCTION_HH
# include <boost/numeric/ublas/symmetric.hpp>

# include <roboptim/core/quadratic-function.hh>

namespace roboptim
{
  namespace ublas = boost::numeric::ublas;

  /// \addtogroup roboptim_function
  /// @{

  /// \brief Build a quadratic function from a matrix and a vector.
  ///
  /// Implement a quadratic function using the general formula:
  /// \f[f(x) = \frac{1}{2} x^t A x + b^t x\f]
  /// where \f$A\f$ and \f$B\f$ are set when the class is instantiated.
  ///
  /// \note A is a symmetric matrix.
  class NumericQuadraticFunction : public QuadraticFunction
  {
  public:
    /// \brief Symmetric matrix type.
    typedef ublas::symmetric_matrix<double, ublas::lower> symmetric_t;

    /// \brief Build a quadratic function from a matrix and a vector.
    ///
    /// See class documentation for A and b definition.
    /// \param A A symmetric matrix
    /// \param b b vector
    NumericQuadraticFunction (const symmetric_t& A, const vector_t& b)
      throw ();

    ~NumericQuadraticFunction () throw ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    void impl_compute (result_t& , const vector_t&) const throw ();
    void impl_gradient (gradient_t&, const vector_t&, size_type = 0)
      const throw ();
    void impl_hessian (hessian_t& hessian,
		       const argument_t& argument,
		       size_type functionId = 0) const throw ();
  private:
    /// \brief A matrix.
    symmetric_t a_;
    /// \brief B vector.
    vector_t b_;
  };

  /// Example shows numeric quadratic function use.
  /// \example numeric-quadratic-function.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
