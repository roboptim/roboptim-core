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
 * \brief Declaration of indentation related methods.
 */

#ifndef ROBOPTIM_CORE_INDENT_HH
# define ROBOPTIM_CORE_INDENT_HH
# include <iosfwd>

namespace roboptim
{
  /// Increment the indentation.
  std::ostream& incindent (std::ostream& o);

  /// Decrement the indentation.
  std::ostream& decindent (std::ostream& o);

  /// Reset the indentation.
  std::ostream& resetindent (std::ostream& o);

  /// Print an end of line, then set the indentation.
  std::ostream& iendl (std::ostream& o);

  /// Increment the indentation, print an end of line, and set the indentation.
  std::ostream& incendl (std::ostream& o);

  /// Decrement the indentation, print an end of line, and set the indentation.
  std::ostream& decendl (std::ostream& o);
}

#endif // !MISC_INDENT_HH
