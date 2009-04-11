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
 * \brief Implementation of the TwiceDerivableFunction class.
 */

#include "liboptimization/twice-derivable-function.hh"
#include "liboptimization/indent.hh"
#include "liboptimization/util.hh"

namespace optimization
{
  TwiceDerivableFunction::TwiceDerivableFunction (size_type n)
    throw ()
    : DerivableFunction (n)
  {
  }

  std::ostream&
  TwiceDerivableFunction::print (std::ostream& o) const throw ()
  {
    return o << "Twice derivable function";
  }
} // end of namespace optimization
