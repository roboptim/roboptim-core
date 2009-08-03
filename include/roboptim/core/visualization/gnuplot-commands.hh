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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <string>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      /// \addtogroup roboptim_visualization
      /// @{

      /// \brief Gnuplot command.
      ///
      /// A command represents a Gnuplot command.
      /// The representation of the command as a string
      /// can be accessed through the #command() method.
      class Command
      {
      public:
	/// \brief Make a command from a string.
	explicit Command (std::string cmd) throw ();

	~Command () throw ();

	/// \brief Retrieve the command as a string.
	const std::string& command () const throw ();
      protected:
	/// \brief Store Gnuplot command.
	std::string command_;
      };

      /// \brief Make a Gnuplot comment.
      Command comment (const char*) throw ();


      /// \brief Make a Gnuplot set command.
      ///
      /// Make a Gnuplot set command (assign a new
      /// value to a variable in Gnuplot).
      /// \param var variable that will be set
      /// \param val new variable value
      /// \return Gnuplot command
      Command set (const char* var, const char* val = "") throw ();

      /// \brief Make a Gnuplot unset command.
      ///
      /// Make a Gnuplot unset command (destroy a variable).
      /// \param var variable that will be unset
      /// \return Gnuplot command
      Command unset (const char* var) throw ();

      /// \brief Make a Gnuplot show command.
      ///
      /// Make a Gnuplot show command (show a variable value).
      /// \param var variable that will be shown
      /// \return Gnuplot command
      Command show (const char* var) throw ();

      /// \brief Make a Gnuplot clear command.
      ///
      /// Make a Gnuplot clear command (clear the screen).
      /// \return Gnuplot command
      Command clear () throw ();

      /// \brief Make a Gnuplot pwdcommand.
      ///
      /// Make a Gnuplot pwd command (show the current directory).
      /// \return Gnuplot command
      Command pwd () throw ();

      /// \brief Make a Gnuplot quit command.
      ///
      /// Make a Gnuplot quit command (quit Gnuplot).
      /// \return Gnuplot command
      Command quit () throw ();

      /// \brief Make a Gnuplot replot command.
      ///
      /// Make a Gnuplot replot command (redraw previously defined curves).
      /// \return Gnuplot command
      Command replot () throw ();

      /// \brief Make a Gnuplot reread command.
      ///
      /// Make a Gnuplot reread command (reread data files).
      /// \return Gnuplot command
      Command reread () throw ();

      /// \brief Make a Gnuplot reset command.
      ///
      /// Make a Gnuplot replot command (reset Gnuplot state).
      /// \return Gnuplot command
      Command reset () throw ();

      /// \brief Make a Gnuplot cd command.
      ///
      /// Make a Gnuplot cd command (change current directory).
      /// \param dir new directory
      /// \return Gnuplot command
      Command cd (const char* dir) throw ();

      /// \brief Make a Gnuplot help command.
      ///
      /// Make a Gnuplot help command (display Gnuplot help).
      /// \param topic help topic
      /// \return Gnuplot command
      Command help (const char* topic = "") throw ();

      /// @}

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
