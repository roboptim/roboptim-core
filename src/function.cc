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
 * \brief Implementation of the Function class.
 */

#include <algorithm>
#include "liboptimization/function.hh"
#include "liboptimization/indent.hh"
#include "liboptimization/util.hh"

namespace optimization
{
  Function::Function (unsigned n) throw ()
    : n (n),
      bound (makeInfiniteBound ()),
      argBounds (n),
      scale (1.),
      argScales (n)
  {
    // Positive size is required.
    assert (n >= 0);

    // Initialize bound.
    for (bounds_t::iterator it = argBounds.begin (); it != argBounds.end (); ++it)
      *it = makeInfiniteBound ();

    // Initialize scale.
    std::fill (argScales.begin (), argScales.end (), 1.);
  }

  Function::~Function () throw ()
  {
  }

  std::ostream&
  Function::print (std::ostream& o) const throw ()
  {
    return o << incindent
             << "Function" << iendl
             << "Result bound: " << bound << iendl
             << "Arguments bounds " << argBounds << iendl
             << "Result scale: " << scale << iendl
             << "Arguments scale: " << argScales
             << decindent;
  }

  std::ostream&
  operator<< (std::ostream& o, const Function& f)
  {
    return f.print (o);
  }
} // end of namespace optimization
