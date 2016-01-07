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

#ifndef ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
# define ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
# include <limits>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/twice-differentiable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Define an abstract quadratic function.
  ///
  /// Inherit from this class when implementing quadratic functions.
  template <typename T>
  class GenericQuadraticFunction : public GenericTwiceDifferentiableFunction<T>
  {
  public:
    typedef GenericTwiceDifferentiableFunction<T> parent_t;
    typedef typename parent_t::size_type size_type;
    ROBOPTIM_ADD_FLAG(ROBOPTIM_IS_QUADRATIC);

    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize function arity
    /// \param outputSize result size
    /// \param name function's name
    GenericQuadraticFunction (size_type inputSize,
			      size_type outputSize = 1,
			      std::string name = std::string ());

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/quadratic-function.hxx>
#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
