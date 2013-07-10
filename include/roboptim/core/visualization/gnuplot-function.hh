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
      template <typename T>
      Command plot (const GenericFunction<T>& f, discreteInterval_t interval);

      /// \brief Plot a 2D function with Gnuplot.
      ///
      /// Plot a 2D function in Gnuplot on a specific interval.
      /// X and Y axises are the function result, the original
      /// arguments are not displayed.
      /// \param f function to be plotted
      /// \param interval plot interval
      /// \return Gnuplot command
      template <typename T>
      Command plot_xy (const GenericFunction<T>& f, discreteInterval_t interval);


      template <typename T>
      Command plot (const GenericFunction<T>& f, discreteInterval_t window)
      {
	assert (f.inputSize () == 1);

	assert (boost::get<0> (window) < boost::get<1> (window)
		&& boost::get<2> (window) > 0.);
	//FIXME: compare with arg bounds?

        std::string str = "";

        if (f.outputSize () == 1)
          str += (boost::format ("plot '-' title '%1%' with line")
                  % f.getName ()).str ();
        else
          str += (boost::format ("plot '-' title '%1% (0)' with line")
                  % f.getName ()).str ();

        for (Function::size_type i = 1; i < f.outputSize (); ++i)
          str += (boost::format (", '-' title '%1% (%2%)' with line")
                  % f.getName ()
                  % i).str ();
	str += "\n";

        // Argument (only 1D supported for now)
	Function::vector_t x (f.inputSize ());
        x.setZero ();

        // Vector used to store the result for each output
        std::vector<std::string> results ((size_t) f.outputSize ());

        for (double t = boost::get<0> (window); t < boost::get<1> (window);
             t += boost::get<2> (window))
          {
            // Evaluate the function
            x[0] = t;
            Function::vector_t res = f (x);

            // Store the result in the vector of strings (for each output)
            for (Function::size_type i = 0; i < f.outputSize (); ++i)
              {
                results [(size_t)i] += (boost::format ("%2.8f %2.8f\n")
                                        % normalize (t)
                                        % normalize (res [i])).str ();
              }
          }

        // Concatenate the result for each output in the final Gnuplot string
        for (std::vector<std::string>::const_iterator it = results.begin();
             it != results.end();
             ++it)
          {
            str += *it + "e\n";
          }

	return Command (str);
      }

      template <typename T>
      Command plot_xy (const GenericFunction<T>& f, discreteInterval_t window)
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
            str += (boost::format ("%2.8f %2.8f\n")
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
