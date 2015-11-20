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
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <roboptim/core/quadratic-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Build a quadratic function from a matrix and a vector.
  ///
  /// Implement a quadratic function using the general formula:
  /// \f[f(x) = x^t A x + b^t x + c\f]
  /// where \f$A\f$ and \f$B\f$ are set when the class is instantiated.
  ///
  /// \note A is a symmetric matrix.
  template <typename T>
  class GenericNumericQuadraticFunction : public GenericQuadraticFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericQuadraticFunction<T>);
    ROBOPTIM_ADD_FLAG(ROBOPTIM_IS_NUMERIC_QUADRATIC);

    /// \brief Symmetric matrix type.
    typedef matrix_t symmetric_t;
    typedef const_matrix_ref const_symmetric_ref;

    /// \brief Build a quadratic function from a matrix and a vector.
    ///
    /// c here is omitted and set to zero.
    ///
    /// See class documentation for A and b definition.
    /// \param A A symmetric matrix
    /// \param b b vector
    /// \param name function's name
    GenericNumericQuadraticFunction (const_symmetric_ref A,
				     const_vector_ref b,
				     std::string name = std::string ());

    /// \brief Build a quadratic function from a matrix and a vector.
    ///
    /// See class documentation for A, b, c definition.
    /// \param A A symmetric matrix (inputSize * inputSize)
    /// \param b b vector (size inputSize)
    /// \param c c vector (size one)
    /// \param name function's name
    GenericNumericQuadraticFunction (const_symmetric_ref A,
				     const_vector_ref b,
				     const_vector_ref c,
				     std::string name = std::string ());


    ~GenericNumericQuadraticFunction ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

    const symmetric_t& A () const
    {
      return a_;
    }

    const vector_t& b () const
    {
      return b_;
    }

    const vector_t& c () const
    {
      return c_;
    }

    symmetric_t& A ()
    {
      return a_;
    }

    vector_t& b ()
    {
      return b_;
    }

    vector_t& c ()
    {
      return c_;
    }

  protected:
    void impl_compute (result_ref, const_argument_ref) const;
    void impl_gradient (gradient_ref, const_argument_ref, size_type = 0)
      const;
    void impl_jacobian (jacobian_ref, const_argument_ref) const;
    void impl_hessian (hessian_ref hessian,
		       const_argument_ref argument,
		       size_type functionId = 0) const;
  private:
    /// \brief A matrix.
    symmetric_t a_;
    /// \brief B vector.
    vector_t b_;
    /// \brief C vector.
    vector_t c_;
    /// \brief buffer to avoid allocating during computation.
    mutable vector_t buffer_;
  };

  /// Example shows numeric quadratic function use.
  /// \example numeric-quadratic-function.cc

  /// @}

} // end of namespace roboptim

# include <roboptim/core/numeric-quadratic-function.hxx>
#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
