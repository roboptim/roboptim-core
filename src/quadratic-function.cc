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
 * \brief Implementation of the QuadraticFunction class.
 */

#include "roboptim-core/quadratic-function.hh"
#include "roboptim-core/indent.hh"
#include "roboptim-core/util.hh"

namespace roboptim
{
  QuadraticFunction::QuadraticFunction (size_type n, size_type m)
    throw ()
    : TwiceDerivableFunction (n, m)
  {
  }

  std::ostream&
  QuadraticFunction::print (std::ostream& o) const throw ()
  {
    return o << "Quadratic function";
  }
} // end of namespace roboptim
