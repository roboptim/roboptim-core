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

#ifndef ROBOPTIM_CORE_NUMERIC_LINEAR_FUNCTION_HH
# define ROBOPTIM_CORE_NUMERIC_LINEAR_FUNCTION_HH

# include <roboptim/core/linear-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Build a linear function from a vector and a matrix.
  ///
  /// Implement a linear function using the general formula:
  /// \f[f(x) = A x + b\f]
  /// where \f$A\f$ and \f$b\f$ are set when the class is instantiated.
  class NumericLinearFunction : public LinearFunction
  {
  public:
    /// \brief Build a linear function from a matrix and a vector.
    ///
    /// See class documentation for A and b definition.
    /// \param A A matrix
    /// \param b b vector
    NumericLinearFunction (const matrix_t& A, const vector_t& b) throw ();
    ~NumericLinearFunction () throw ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    void impl_compute (result_t& , const vector_t&) const throw ();
    void impl_gradient (gradient_t&, const vector_t&, int = 0) const throw ();
    void impl_jacobian (jacobian_t&, const vector_t&) const throw ();

  private:
    /// \brief A matrix.
    matrix_t a_;
    /// \brief B vector.
    vector_t b_;
  };

  /// Example shows numeric linear function use.
  /// \example numeric-linear-function.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
