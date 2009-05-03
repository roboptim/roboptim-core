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


/**
 * \brief Useful procedures declaration.
 */
#ifndef ROBOPTIM_CORE_UTIL_HH
# define ROBOPTIM_CORE_UTIL_HH
# include <boost/variant.hpp>
# include <roboptim/core/derivable-function.hh>

namespace roboptim
{
  namespace detail
  {
    /// \internal
    /// Copy the content of a uBLAS vector into a C array.
    void vector_to_array (Function::value_type* dst, const Function::vector_t& src);

    /// \internal
    /// Copy the content of a C array into a uBLAS vector.
    void array_to_vector (Function::vector_t& dst, const Function::value_type* src);

    /// \internal
    /// Merge gradients from several functions (each gradient is a line).
    /// The first line of the jacobian is the only one used.
    template <typename T>
    void
    jacobian_from_gradients (DerivableFunction::matrix_t& jac,
                             const std::vector<const T*>& c,
                             const DerivableFunction::vector_t& x);
  } // end of namespace detail.

  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream&, const boost::variant<T1, T2>&);

  template <typename T>
  std::ostream& operator<< (std::ostream&, const std::vector<T>&);

  template <typename T1, typename T2>
  std::ostream& operator<< (std::ostream&, const std::pair<T1, T2>&);
} // end of namespace roboptim.

# include <roboptim/core/util.hxx>
#endif //! ROBOPTIM_CORE_UTIL_HH
