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

#ifndef ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_HH
# define ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>
# include <roboptim/core/function.hh>

# include <utility>
# include <vector>

# define EIGEN_YES_I_KNOW_SPARE_MODULE_IS_NOT_STABLE_YET
# include <Eigen/Core>
# include <Eigen/Sparse>

# include <roboptim/core/function.hh>
# include <roboptim/core/visualization/fwd.hh>
# include <roboptim/core/visualization/matplotlib-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    /// \addtogroup roboptim_visualization
    /// @{

    /// \brief matplotlib script
    ///
    /// This class gathers matplotlib commands
    /// to build a complete matplotlib script.
    /// matplotlib commands can be inserted through
    /// this object through the \c << operator
    /// and this object can be put into an output stream
    /// using the \c << operator.
    class ROBOPTIM_CORE_DLLAPI Matplotlib
    {
    public:
      ~Matplotlib ();

      /// \brief Instanciate a matplotlib without setting a term.
      /// \param with_header whether to print the header or not
      /// \return matplotlib instance
      static Matplotlib make_matplotlib (std::pair<int, int> multiplot = std::make_pair(1, 1), bool with_header = true)
      {
	return Matplotlib (multiplot, with_header);
      }

      /// \brief Add a new matplotlib command to the script.
      /// \param cmd command that will be pushed
      void push_command (const matplotlib::Command& cmd);

      /// \brief Add a new Python import to the script.
      /// \param import package to import
      void push_import (const matplotlib::Import& import);

      /// \brief Display the matplotlib script on the specified output stream.
      ///
      /// \param o output stream used for display
      /// \return output stream
      std::ostream& print (std::ostream&) const;

      Matplotlib& operator << (matplotlib::Command);
      Matplotlib& operator << (matplotlib::Import);

      /// \brief Clear the vectors of imports and commands.
      void clear ();

      /// \brief Whether to print the header (imports).
      /// \return Reference to withHeader_.
      bool& withHeader ();

      /// \brief Whether to print the header (imports).
      bool withHeader () const;

      /// \brief Plots layout. {1,1} if unspecified.
      /// \return Reference to multiplot_
      std::pair<int, int>& multiplot ();

      /// \brief Plots layout. {1,1} if unspecified.
      std::pair<int, int> multiplot () const;
    protected:
      /// \brief Default constructor can not be called directly.
      ///
      /// Use of the named constructor (see static methods) to
      /// instantiate this class.
      explicit Matplotlib (std::pair<int, int> multiplot = std::make_pair(1, 1), bool with_header = true);

      /// \brief Reset imports to initial values.
      void resetImports ();

    private:
      /// \brief Vector of imports.
      std::vector<matplotlib::Import> imports_;

      /// \brief Vector of commands.
      std::vector<matplotlib::Command> commands_;

      /// \brief Whether to export the header (imports etc.).
      bool withHeader_;

      /// \brief Whether to expect multiple plots or not.
      std::pair<int, int> multiplot_;
    };

    /// Example shows simple matplotlib visualization.
    /// \example visualization-matplotlib-simple.cc

    /// Example shows function display with matplotlib.
    /// \example visualization-matplotlib-function.cc

    /// \brief Override operator<< to handle matplotlib command insertion.
    ///
    /// \tparam Matplotlib command type
    /// \param gp matplotlib script that will receive the new command
    /// \param t new matplotlib command.
    /// \return modified matplotlib script
    template <typename T>
    Matplotlib& operator<< (Matplotlib& ml, T t)
    {
      ml.push_command (t ());
      return ml;
    }

    /// @}

    /// \brief Override operator<< to handle matplotlib script display.
    ///
    /// \param o output stream used for display
    /// \param ml matplotlib script to be displayed
    /// \return output stream
    ROBOPTIM_CORE_DLLAPI std::ostream& operator<< (std::ostream&, const Matplotlib& ml);
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_HH
