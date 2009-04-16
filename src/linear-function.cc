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
 * \brief Implementation of the LinearFunction class.
 */

#include "roboptim-core/linear-function.hh"
#include "roboptim-core/indent.hh"
#include "roboptim-core/util.hh"

namespace roboptim
{
  LinearFunction::LinearFunction (size_type n, size_type m) throw ()
    : QuadraticFunction (n, m)
  {
  }

  LinearFunction::hessian_t
  LinearFunction::hessian (const vector_t&, int) const throw ()
  {
    hessian_t h (n,n);
    h.clear ();
    return h;
  }

  std::ostream&
  LinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Linear function";
  }
} // end of namespace roboptim
