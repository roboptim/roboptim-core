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
 * \file src/function.cc
 *
 * \brief Implementation of the Function class.
 */

#include "function.hh"

namespace optimization
{
  Function::Function (unsigned n, value_type inf) throw ()
    : n (n),
      infinity (inf),
      argBounds (n)
  {
    bound = std::make_pair (-inf, inf);

    for (bounds_t::iterator it = argBounds.begin (); it != argBounds.end (); ++it)
      *it = std::make_pair (-inf, inf);
  }

  Function::~Function () throw ()
  {
  }

  Function::gradient_t
  Function::gradient (const vector_t&) const throw ()
  {
    return gradient_t ();
  }

  Function::hessian_t
  Function::hessian (const vector_t&) const throw ()
  {
    return hessian_t ();
  }
} // end of namespace optimization
