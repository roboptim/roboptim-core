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
 * \brief Gnuplot visualization.
 */

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# include <string>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      class Command
      {
      public:
	explicit Command (std::string cmd) throw ();
	~Command () throw ();
	const std::string& command () const throw ();
      protected:
	std::string command_;
      };

      Command comment (const char*) throw ();
      Command set (const char*, const char*  = "") throw ();
      Command unset (const char*) throw ();
      Command show (const char*, const char*) throw ();

      Command clear () throw ();
      Command pwd () throw ();
      Command quit () throw ();
      Command replot () throw ();
      Command reread () throw ();
      Command reset () throw ();

      Command cd (const char*) throw ();
      Command help (const char* = "") throw ();

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
