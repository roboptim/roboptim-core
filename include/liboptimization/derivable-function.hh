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
 * \brief Declaration of the DerivableFunction class.
 */

#ifndef OPTIMIZATION_DERIVABLE_FUNCTION_HH
# define OPTIMIZATION_DERIVABLE_FUNCTION_HH
# include <limits>

# include <liboptimization/function.hh>

namespace optimization
{
  /// Define a derivable function.
  class DerivableFunction : public Function
  {
  public:
    /// Gradient type.
    typedef vector_t gradient_t;

    DerivableFunction (size_type n, value_type infinity =
                       std::numeric_limits<value_type>::infinity ()) throw ();

    /// Gradient.
    virtual gradient_t gradient (const vector_t&) const throw () = 0;
  };

}; // end of namespace optimization

#endif //! OPTIMIZATION_DERIVABLE_FUNCTION_HH
