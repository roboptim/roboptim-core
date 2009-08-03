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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <vector>

# include <roboptim/core/visualization/fwd.hh>
# include <roboptim/core/visualization/gnuplot-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    /// \addtogroup roboptim_visualization
    /// @{

    /// \brief Gnuplot script
    ///
    /// This class gathers Gnuplot commands
    /// to build a complete Gnuplot script.
    /// Gnuplot commands can be inserted through
    /// this object through the \c << operator
    /// and this object can be put into an output stream
    /// using the \c << operator.
    class Gnuplot
    {
    public:
      ~Gnuplot () throw ();

      /// \brief Instanciate a Gnuplot without setting a term.
      /// \return Gnuplot instance
      static Gnuplot make_gnuplot () throw ()
      {
	return Gnuplot ();
      }

      /// \brief Instanciate a Gnuplot suitable for interactive use.
      ///
      /// This initializes a persistent Gnuplot instance which are
      /// suitable for user interaction.
      /// \return Gnuplot instance
      static Gnuplot make_interactive_gnuplot () throw ()
      {
	Gnuplot gp;
	gp.push_command (gnuplot::set ("terminal", "wxt persist"));
	return gp;
      }

      /// \brief Add a new Gnuplot command to the script.
      /// \param cmd command that will be pushed
      void push_command (gnuplot::Command cmd) throw ();


      /// \brief Display the Gnuplot script on the specified output stream.
      ///
      /// \param o output stream used for display
      /// \return output stream
      std::ostream& print (std::ostream&) const throw ();

      Gnuplot& operator << (gnuplot::Command) throw ();

    protected:
      /// \brief Default constructor can not be called directly.
      ///
      /// Use of the named constructor (see static methods) to
      /// instantiate this class.
      explicit Gnuplot () throw ();
    private:
      /// \brief Vector of commands.
      std::vector<gnuplot::Command> commands_;
    };

    /// Example shows simple Gnuplot visualization.
    /// \example visualization-gnuplot-simple.cc

    /// Example shows function display with Gnuplot.
    /// \example visualization-gnuplot-function.cc

    /// \brief Override operator<< to handle Gnuplot command insertion.
    ///
    /// \tparam Gnuplot command type
    /// \param gp Gnuplot script that will receive the new command
    /// \param t new Gnuplot command.
    /// \return modified Gnuplot script
    template <typename T>
    Gnuplot& operator<< (Gnuplot& gp, T t)
    {
      gp.push_command (t ());
      return gp;
    }

    /// @}

    /// \brief Override operator<< to handle Gnuplot script display.
    ///
    /// \param o output stream used for display
    /// \param gp Gnuplot script to be displayed
    /// \return output stream
    std::ostream& operator<< (std::ostream&, const Gnuplot& gp);
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
