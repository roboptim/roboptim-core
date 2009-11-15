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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_FUNCTION_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_FUNCTION_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <boost/format.hpp>

# include <roboptim/core/function.hh>

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

      /// \brief Plot a 1D function with Gnuplot.
      ///
      /// Plot a 1D function in Gnuplot on a specific interval.
      /// X axis is the function's argument, Y axis is result.
      /// \param f function to be plotted
      /// \param interval plot interval
      /// \return Gnuplot command
      Command plot (const Function& f, discreteInterval_t interval);

      /// \brief Plot a 2D function with Gnuplot.
      ///
      /// Plot a 2D function in Gnuplot on a specific interval.
      /// X and Y axises are the function result, the original
      /// arguments are not displayed.
      /// \param f function to be plotted
      /// \param interval plot interval
      /// \return Gnuplot command
      Command plot_xy (const Function& f, discreteInterval_t interval);


      Command plot (const Function& f, discreteInterval_t window)
      {
	assert (f.inputSize () == 1);

	assert (boost::get<0> (window) < boost::get<1> (window)
		&& boost::get<2> (window) > 0.);
	//FIXME: compare with arg bounds?

	std::string str = (boost::format ("plot '-' title '%1%' with line")
			   % f.getName ()).str ();

	for (unsigned i = 1; i < f.outputSize (); ++i)
	  str += ", '-' with line";
	str += "\n";

	Function::vector_t x (f.inputSize ());
	for (unsigned i = 0; i < f.outputSize (); ++i)
	  {
	    for (double t = boost::get<0> (window); t < boost::get<1> (window);
		 t += boost::get<2> (window))
	      {
		x[0] = t;
		Function::vector_t res = f (x);
		str += (boost::format ("%1.2f %2.2f\n")
			% normalize (t)
			% normalize (res [0])).str ();
	      }
	    str += "e\n";
	  }

	return Command (str);
      }


      Command plot_xy (const Function& f, discreteInterval_t window)
      {
	assert (f.inputSize () == 1 && f.outputSize () == 2);

	assert (boost::get<0> (window) < boost::get<1> (window)
		&& boost::get<2> (window) > 0.);
	//FIXME: compare with arg bounds?

	std::string str = (boost::format ("plot '-' title '%1%' with line\n")
			   % f.getName ()).str ();

	Function::vector_t x (f.inputSize ());

	for (double t = boost::get<0> (window); t < boost::get<1> (window);
	     t += boost::get<2> (window))
	  {
	    x[0] = t;
	    Function::vector_t res = f (x);
	    str += (boost::format ("%1.2f %2.2f\n")
		    % normalize (res[0])
		    % normalize (res [1])).str ();
	  }
	str += "e\n";

	return Command (str);
      }

      /// @}
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_FUNCTION_HH
