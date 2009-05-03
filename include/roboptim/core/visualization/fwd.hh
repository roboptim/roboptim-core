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
 * \brief Visualization forward declarations.
 */

#ifndef ROBOPTIM_CORE_VISUALIZATION_FWD_HH
# define ROBOPTIM_CORE_VISUALIZATION_FWD_HH

namespace roboptim
{
  namespace visualization
  {
    class Gnuplot;

    namespace gnuplot
    {
      class Command;

      class comment;
      class clear;
      class pwd;
      class quit;
      class replot;
      class reread;
      class reset;

      class cd;
      class help;

      class plot;

    } // end of namespace gnuplot.

  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_HH
