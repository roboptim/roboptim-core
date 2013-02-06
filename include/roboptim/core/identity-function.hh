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

#ifndef ROBOPTIM_CORE_IDENTITY_FUNCTION_HH
# define ROBOPTIM_CORE_IDENTITY_FUNCTION_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <roboptim/core/linear-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Identity function.
  ///
  /// Implement a linear function using the formula:
  /// \f[f(x) = x + offset\f]
  /// where \f$A\f$ and \f$b\f$ are set when the class is instantiated.
  class ROBOPTIM_DLLAPI IdentityFunction : public LinearFunction
  {
  public:
    /// \brief Build an identity function.
    ///
    /// \param offset identity function offset
    IdentityFunction (const vector_t& offset) throw ();
    ~IdentityFunction () throw ();

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    void impl_compute (result_t , argument_t) const throw ();
    void impl_gradient (gradient_t, argument_t, size_type = 0)
      const throw ();
    void impl_jacobian (jacobian_t, argument_t) const throw ();

  private:
    const vector_t offset_;
  };

  /// Example shows identity function use.
  /// \example identity-function.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_IDENTITY_FUNCTION_HH
