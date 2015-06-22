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

#ifndef ROBOPTIM_CORE_LINEAR_FUNCTION_HH
# define ROBOPTIM_CORE_LINEAR_FUNCTION_HH
# include <limits>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/portability.hh>
# include <roboptim/core/quadratic-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Define an abstract linear function.
  ///
  /// Inherit from this class when implementing linear functions.
  template <typename T>
  class GenericLinearFunction : public GenericQuadraticFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericQuadraticFunction<T>);

    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize function arity
    /// \param outputSize result size
    /// \param name function's name
    GenericLinearFunction (size_type inputSize,
			   size_type outputSize = 1,
			   std::string name = std::string ());

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;
  protected:
    void impl_hessian (hessian_ref hessian,
		       const_argument_ref argument,
		       size_type functionId = 0) const;

  public:
    /// \brief Get the type-checking flag
    virtual unsigned long flag()
    {
      return flag_;
    }

    /// \brief Flag representing the Roboptim Function type
    static unsigned long flag_;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/linear-function.hxx>
#endif //! ROBOPTIM_CORE_LINEAR_FUNCTION_HH
