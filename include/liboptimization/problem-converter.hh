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
 * \brief Declaration of the problem_converter function.
 */

#ifndef OPTIMIZATION_PROBLEM_CONVERTER_HH
# define OPTIMIZATION_PROBLEM_CONVERTER_HH
# include <stdexcept>
# include <boost/static_assert.hpp>
# include <boost/type_traits/is_convertible.hpp>

# include <liboptimization/fwd.hh>
# include <liboptimization/problem.hh>

namespace optimization
{
  /// Try to convert a problem of a type to another type.
  /// If fail, throw a std::bad_cast.
  template <typename PBSRC, typename PBDST>
  PBDST problem_converter (const PBSRC& src) throw (std::bad_cast);
}; // end of namespace optimization

# include <liboptimization/problem-converter.hxx>
#endif //! OPTIMIZATION_PROBLEM_CONVERTER_HH
