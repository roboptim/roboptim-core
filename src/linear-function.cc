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
#include "liboptimization/indent.hh"
#include "liboptimization/util.hh"

namespace optimization
{
  LinearFunction::LinearFunction (size_type n, size_type m) throw ()
    : QuadraticFunction (n, m)
  {
  }

  LinearFunction::hessian_t
  LinearFunction::hessian (const vector_t&) const throw ()
  {
    using namespace boost::numeric::ublas;
    hessian_t h (n, n);
    h.clear ();
    return h;
  }

  std::ostream&
  LinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Linear function";
  }
} // end of namespace optimization
