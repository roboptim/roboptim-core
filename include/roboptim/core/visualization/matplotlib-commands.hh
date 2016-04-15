// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_COMMANDS_HH
# define ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_COMMANDS_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>
# include <roboptim/core/util.hh>

# include <string>
# include <iostream>

namespace roboptim
{
  namespace visualization
  {
    namespace matplotlib
    {
      /// \addtogroup roboptim_visualization
      /// @{

      /// \brief matplotlib import.
      ///
      /// This class represents a Python import command.
      class ROBOPTIM_CORE_DLLAPI Import
      {
      public:
        /// \brief Construct from a Python package.
        /// \param packages packages to import (separated by commas).
        Import (const std::string& package);

        /// \brief Construct from a Python package. "from-import" version.
        /// \param from Python module.
        /// \param packages packages to import (separated by commas).
        Import (const std::string& from, const std::string& packages);

        /// \brief Import destructor.
        ~Import ();

        /// \brief Return the import command.
        /// \return import command.
        const std::string& command () const
        {
          return command_;
        }

      private:
        /// \brief Import command.
        std::string command_;
      };

      /// \brief Import function that follows the style of commands.
      ROBOPTIM_CORE_DLLAPI Import import (const std::string& packages);

      /// \brief Import function that follows the style of commands.
      ROBOPTIM_CORE_DLLAPI Import import (const std::string& from,
                                     const std::string& packages);

      /// \brief matplotlib command.
      ///
      /// A command represents a matplotlib command.
      /// The representation of the command as a string
      /// can be accessed through the #command() method.
      class ROBOPTIM_CORE_DLLAPI Command
      {
      public:
	/// \brief Make a command from a string.
	explicit Command (const std::string& cmd, bool isPlot = false);

	~Command ();

	/// \brief Retrieve the command as a string.
	const std::string& command () const;

	const bool& isPlot() const;

      protected:
	/// \brief Store matplotlib command.
	std::string command_;
	/// \brief Whether the command is a plot or not
	bool isPlot_;
      };

      /// \brief Make a matplotlib comment.
      /// \throw std::runtime_error
      ROBOPTIM_CORE_DLLAPI Command comment (const char*);

      /// \brief Make a matplotlib comment.
      /// \throw std::runtime_error
      template <typename T>
      inline Command comment (const T& content)
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
        ss << content;

        return comment (ss.str ().c_str ());
      }

      /// \brief Make a matplotlib set command.
      ///
      /// Make a matplotlib set command (assign a new
      /// value to a variable in matplotlib).
      /// \param var variable that will be set
      /// \param val new variable value
      /// \return matplotlib command
      ROBOPTIM_CORE_DLLAPI Command set (const char* var, const char* val);

      ROBOPTIM_CORE_DLLAPI Command show ();
      ROBOPTIM_CORE_DLLAPI Command figure ();
      ROBOPTIM_CORE_DLLAPI Command title (const char* argument);

      /// @}

    } // end of namespace matplotlib.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_COMMANDS_HH
