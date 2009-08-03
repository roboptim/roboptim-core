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

#include "debug.hh"

#include "roboptim/core/function.hh"
#include "roboptim/core/indent.hh"
#include "roboptim/core/util.hh"

namespace roboptim
{
  Function::Function (size_type inputSize,
		      size_type outputSize,
		      std::string name) throw ()
    : inputSize_ (inputSize),
      outputSize_ (outputSize),
      name_ (name)
  {
    // Positive size is required.
    assert (inputSize > 0 && outputSize > 0);
  }

  Function::~Function () throw ()
  {
  }

  std::ostream&
  Function::print (std::ostream& o) const throw ()
  {
    if (getName ().empty ())
      return o << "Function";
    else
      return o << getName  () << " (not derivable)";
  }

  std::ostream&
  operator<< (std::ostream& o, const Function& f)
  {
    return f.print (o);
  }
} // end of namespace roboptim
