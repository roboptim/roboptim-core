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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_DIFFERENTIABLE_FUNCTION_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_DIFFERENTIABLE_FUNCTION_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <boost/format.hpp>
# include <boost/lexical_cast.hpp>

# include <roboptim/core/function.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      /// \addtogroup roboptim_visualization
      /// @{


      /// \brief Import discrete interval type from function.
      typedef Function::discreteInterval_t discreteInterval_t;

      /// \brief Import discrete interval type from function.
      typedef DifferentiableFunction::argument_t argument_t;

      /// \brief Plot the Jacobian structure with Gnuplot.
      ///
      /// Plot the structure of the Jacobian of a differentiable function with
      /// Gnuplot. Non-zero values will be displayed in blue, zeros in white.
      ///
      /// \param f differentiable function whose Jacobian will be plotted
      /// \param arg optimization parameters of the point to plot
      /// \return Gnuplot command
      Command plot_jac (const DifferentiableFunction& f,
                        const DifferentiableFunction::argument_t& arg);

      Command plot_jac (const DifferentiableFunction& f,
                        const argument_t& arg)
      {

	DifferentiableFunction::jacobian_t jac = f.jacobian(arg);

	std::string str = "";

	// Title of the graph
	str += "set title 'jacobian(" + f.getName() + ")'\n";

	// White = 0, Blue = non zero
	str += "set palette defined(0 \"white\",1 \"blue\")\n";
	str += "set grid front\n";

	// Jacobian (x,y) range
	str += "set xrange [0:" + boost::lexical_cast<std::string>
	  ((float)jac.cols()) + "]\n";
	str += "set yrange [0:" + boost::lexical_cast<std::string>
	  ((float)jac.rows()) + "] reverse\n";
	str += "set size ratio -1\n";

	// Remove the colorbox
	str += "unset colorbox\n";

	// Matrix plotting
	// (range offset since pixels are centered on integer coordinates)
	str += "plot '-' using ($1+0.5):($2+0.5):($3 == 0 ? 0 : 1) ";
	str += "matrix with image notitle\n";

	for (DifferentiableFunction::jacobian_t::Index cstr_id = 0;
	     cstr_id < jac.rows(); ++cstr_id)
	  for (DifferentiableFunction::jacobian_t::Index out_id = 0;
	       out_id < jac.cols(); ++out_id)
	    {
	      str += (boost::format("%1.2f")
		      % normalize(jac(cstr_id, out_id))).str();

              if (out_id < jac.cols() - 1) str += " ";
              else str += "\n";
	    }
	str += "e\n";

	return Command (str);
      }

      /// @}
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_DIFFERENTIABLE_FUNCTION_HH
