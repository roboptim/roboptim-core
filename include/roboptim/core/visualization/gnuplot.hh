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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
# include <vector>

# include <roboptim/core/visualization/fwd.hh>
# include <roboptim/core/visualization/gnuplot-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    class Gnuplot
    {
    public:
      ~Gnuplot () throw ();

      /// Instanciate a Gnuplot without setting a term.
      static Gnuplot make_gnuplot () throw ()
      {
	return Gnuplot ();
      }

      /// Instanciate a Gnuplot suitable for interactive use.
      /// (i.e. being seen by an user, not just rendering an image).
      static Gnuplot make_interactive_gnuplot () throw ()
      {
	Gnuplot gp;
	gp.push_command (gnuplot::set ("terminal", "wxt persist"));
	return gp;
      }

      void push_command (gnuplot::Command) throw ();

      std::ostream& print (std::ostream&) const throw ();

      Gnuplot& operator << (gnuplot::Command) throw ();

    protected:
      explicit Gnuplot () throw ();
    private:
      std::vector<gnuplot::Command> commands_;
    };

    template <typename T>
    Gnuplot& operator<< (Gnuplot& gp, T t)
    {
      gp.push_command (t ());
      return gp;
    }

    std::ostream& operator<< (std::ostream&, const Gnuplot&);
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
