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
# include <roboptim/core/util.hh>

# include <string>
# include <iostream>

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
      class ROBOPTIM_DLLAPI Command
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
      ROBOPTIM_DLLAPI Command comment (const char*)
	throw (std::runtime_error);

      /// \brief Make a Gnuplot comment.
      template <typename T>
      inline Command comment (const T& content) throw ()
      {
        // Note: we do not use boost::lexical_cast because the << operators
        // need to be in the std:: or boost:: namespaces. As a result, if we
        // try to add a comment with an Eigen matrix, it will not be printed
        // in the RobOptim way. Thus, we stick to stringstream (for now).
        // Also, we use the "using" keyword to force the use of the RobOptim
        // << operator when it is available (e.g. for Eigen matrices, pairs
        // of values, etc.).
        using roboptim::operator <<;
        using std::operator <<;

        std::stringstream ss;
        ss << "# " << content;

        return Command (ss.str ());
      }

      /// \brief Make a Gnuplot set command.
      ///
      /// Make a Gnuplot set command (assign a new
      /// value to a variable in Gnuplot).
      /// \param var variable that will be set
      /// \param val new variable value
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command set (const char* var, const char* val = "")
	throw (std::runtime_error);

      /// \brief Make a Gnuplot unset command.
      ///
      /// Make a Gnuplot unset command (destroy a variable).
      /// \param var variable that will be unset
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command unset (const char* var)
	throw (std::runtime_error);

      /// \brief Make a Gnuplot show command.
      ///
      /// Make a Gnuplot show command (show a variable value).
      /// \param var variable that will be shown
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command show (const char* var)
	throw (std::runtime_error);

      /// \brief Make a Gnuplot clear command.
      ///
      /// Make a Gnuplot clear command (clear the screen).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command clear () throw ();

      /// \brief Make a Gnuplot pwdcommand.
      ///
      /// Make a Gnuplot pwd command (show the current directory).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command pwd () throw ();

      /// \brief Make a Gnuplot quit command.
      ///
      /// Make a Gnuplot quit command (quit Gnuplot).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command quit () throw ();

      /// \brief Make a Gnuplot replot command.
      ///
      /// Make a Gnuplot replot command (redraw previously defined curves).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command replot () throw ();

      /// \brief Make a Gnuplot reread command.
      ///
      /// Make a Gnuplot reread command (reread data files).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command reread () throw ();

      /// \brief Make a Gnuplot reset command.
      ///
      /// Make a Gnuplot replot command (reset Gnuplot state).
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command reset () throw ();

      /// \brief Make a Gnuplot cd command.
      ///
      /// Make a Gnuplot cd command (change current directory).
      /// \param dir new directory
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command cd (const char* dir) throw ();

      /// \brief Make a Gnuplot help command.
      ///
      /// Make a Gnuplot help command (display Gnuplot help).
      /// \param topic help topic
      /// \return Gnuplot command
      ROBOPTIM_DLLAPI Command help (const char* topic = "") throw ();

      /// @}

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
