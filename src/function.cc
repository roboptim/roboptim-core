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

#include <algorithm>
#include "function.hh"

namespace optimization
{
  Function::Function (unsigned n, value_type inf) throw ()
    : n (n),
      infinity (inf),
      bound (std::make_pair (-inf, inf)),
      argBounds (n),
      scale (1.),
      argScales (n)
  {
    // A positive infinite is required.
    assert (inf > 0.);

    // Initialize bound.
    for (bounds_t::iterator it = argBounds.begin (); it != argBounds.end (); ++it)
      *it = std::make_pair (-inf, inf);

    // Initialize scale.
    std::fill (argScales.begin (), argScales.end (), 1.);
  }

  Function::~Function () throw ()
  {
  }

  Function::bound_t
  Function::makeBound (value_type l, value_type u) const
    throw ()
  {
    return std::make_pair (l, u);
  }

  Function::bound_t
  Function::makeBound () const
    throw ()
  {
    return std::make_pair (-infinity, infinity);
  }

  Function::bound_t
  Function::makeUpperBound (value_type u) const
  throw ()
  {
    return makeBound (-infinity, u);
  }

  Function::bound_t
  Function::makeLowerBound (value_type l) const
  throw ()
  {
    return makeBound (l, infinity);
  }


  DerivableFunction::DerivableFunction (size_type n, value_type infinity)
    throw ()
    : Function (n, infinity)
  {
  }

  TwiceDerivableFunction::TwiceDerivableFunction
  (size_type n, value_type infinity)
    throw ()
    : DerivableFunction (n, infinity)
  {
  }

  QuadraticFunction::QuadraticFunction (size_type n, value_type infinity)
    throw ()
    : TwiceDerivableFunction (n, infinity)
  {
  }

  LinearFunction::LinearFunction (size_type n, value_type infinity) throw ()
    : QuadraticFunction (n, infinity)
  {
  }

} // end of namespace optimization
