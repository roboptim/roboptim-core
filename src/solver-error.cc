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

#include <string>
#include <sstream>
#include <vector>

#include "debug.hh"

#include "roboptim/core/solver-error.hh"
#include "roboptim/core/util.hh"
#include "roboptim/core/terminal-color.hh"

namespace roboptim
{
  SolverError::SolverError (const std::string& msg)
    : std::runtime_error (msg)
  {
  }

  SolverError::SolverError (const std::string& msg,
                            const Result& res)
    : std::runtime_error (msg),
      lastState_(res)
  {
  }

  SolverError::SolverError (const SolverError& error)
    : std::runtime_error (error.what()),
      lastState_(error.lastState_)
  {
  }

  SolverError::~SolverError () throw ()
  {
  }

  std::ostream&
  SolverError::print (std::ostream& o) const
  {
    o << fg::fail << "Solver error";

    // If the error message is not empty
    std::string msg (what ());
    if (!msg.empty ())
    {
      o << ":";

      // Split string on newlines, and insert indenting
      std::vector<std::string> errors = split (msg, '\n');
      if (errors.size () == 1)
      {
        o << " " << errors[0];
      }
      else
      {
        o << incindent;
        for (std::vector<std::string>::const_iterator
            s = errors.begin (); s != errors.end (); ++s)
        {
          o << iendl << *s;
        }
        o << decindent;
      }
    }
    o << fg::reset;

    return o;
  }

  const boost::optional<Result>& SolverError::lastState () const
  {
    return lastState_;
  }

  boost::optional<Result>& SolverError::lastState ()
  {
    return lastState_;
  }

  std::ostream& operator<< (std::ostream& o, const SolverError& s)
  {
    return s.print (o);
  }

} // end of namespace roboptim
