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
 * \brief Implementation of the LinearFunction class.
 */

#include "liboptimization/linear-function.hh"
#include "liboptimization/util.hh"

namespace optimization
{
  LinearFunction::LinearFunction (size_type n, value_type infinity) throw ()
    : QuadraticFunction (n, infinity)
  {
  }

  LinearFunction::hessian_t
  LinearFunction::hessian (const vector_t&) const throw ()
  {
    LinearFunction::hessian_t h (n, n);
    h.clear ();
    return h;
  }

  std::ostream&
  LinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Linear function" << std::endl
             << "Result bound: " << bound << std::endl
             << "Arguments bounds " << argBounds << std::endl
             << "Result scale: " << scale << std::endl
             << "Arguments scale: " << argScales;
  }
} // end of namespace optimization
