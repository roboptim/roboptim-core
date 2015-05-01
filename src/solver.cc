// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#include "roboptim/core/solver.hh"

namespace roboptim
{
  namespace detail
  {
    struct PrintSolverParameter : public boost::static_visitor<std::ostream&>
    {
      PrintSolverParameter (std::ostream& o) : o_ (o)
      {}

      template <typename T>
      std::ostream& operator () (const T& val)
      {
        o_ << val;
        return o_;
      }

      std::ostream& operator () (const bool& val)
      {
        o_ << (val? "true" : "false");
        return o_;
      }

    private:
      std::ostream& o_;
    };
  } // end of namespace detail

  Parameter::Parameter ()
    : description (""),
      value ()
  {}

  std::ostream& operator<< (std::ostream& o,
			    const Parameter& parameter)
  {
    detail::PrintSolverParameter psp (o);

    if (!parameter.description.empty ())
      o << " (" << parameter.description << ")";
    o << ": ";

    boost::apply_visitor (psp, parameter.value);

    return o;
  }
} // end of namespace roboptim
