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

#include "roboptim/core/twice-derivable-function.hh"
#include "roboptim/core/indent.hh"
#include "roboptim/core/util.hh"

namespace roboptim
{
  TwiceDerivableFunction::TwiceDerivableFunction
  (size_type inputSize,
   size_type outputSize,
   std::string name) throw ()
    : DerivableFunction (inputSize, outputSize, name)
  {
  }

  std::ostream&
  TwiceDerivableFunction::print (std::ostream& o) const throw ()
  {
    if (getName ().empty ())
      return o << "Twice derivable function";
    else
      return o << getName () << " (twice derivable function)";
  }
} // end of namespace roboptim
