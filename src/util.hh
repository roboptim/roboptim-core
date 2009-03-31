// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file src/util.hh
 *
 * \brief Useful procedures declaration.
 */
#ifndef OPTIMIZATION_UTIL_HH
# define OPTIMIZATION_UTIL_HH
# include <solver.hh>

namespace optimization
{
  namespace detail
  {
    /// Copy the content of a uBLAS vector into a C array.
    void vector_to_array (Solver::value_type* dst, const Solver::array_t& src);

    /// Copy the content of a C array into a uBLAS vector.
    void array_to_vector (Solver::array_t& dst, const Solver::value_type* src);

  }; // end of namespace detail.
}; // end of namespace optimization.

#endif //! OPTIMIZATION_UTIL_HH
