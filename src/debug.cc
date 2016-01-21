// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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

#include <iostream>
#include <cstdlib>

#include "roboptim/core/debug.hh"
#include "roboptim/core/terminal-color.hh"

namespace roboptim
{
  void assertion_failed(char const *expr, char const *function,
                        char const *file, long line)
  {
    std::cerr << fg::bold << "Assertion" << fg::reset
              << ": `" << fg::fail << fg::bold << expr << fg::reset
              << "` failed in " << file << ":" << line
              << ": " << function << std::endl;
    std::abort();
  }

  void assertion_failed_msg(char const *expr, char const *msg,
                            char const *function, char const *file, long line)
  {
    std::cerr << fg::bold << "Assertion" << fg::reset
              << ": `" << fg::fail << fg::bold << expr << fg::reset
              << "` failed in " <<  fg::bold << file << ":" << line << fg::reset
              << ": " << function << ": "
              << fg::warn << fg::bold << msg << fg::reset << std::endl;
    std::abort();
  }
}
