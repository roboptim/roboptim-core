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

#ifndef ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_FUNCTION_HH
# define ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_FUNCTION_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <boost/format.hpp>

# include <roboptim/core/function.hh>
# include <roboptim/core/visualization/matplotlib-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace matplotlib
    {
      /// \addtogroup roboptim_visualization
      /// @{

      /// \brief Import discrete interval type from function.
      typedef Function::discreteInterval_t discreteInterval_t;

      /// \brief Plot a 1D function with matplotlib.
      ///
      /// Plot a 1D function in matplotlib on a specific interval.
      /// X axis is the function's argument, Y axis is result.
      /// \param f function to be plotted
      /// \param interval plot interval
      /// \return matplotlib command
      template <typename T>
      Command plot (const GenericFunction<T>& f, discreteInterval_t interval);


      template <typename T>
      Command plot (const GenericFunction<T>& f, discreteInterval_t window)
      {
        typedef typename GenericFunction<T>::value_type value_type;

        assert (f.inputSize () == 1);

        assert (boost::get<0> (window) < boost::get<1> (window)
                && boost::get<2> (window) > 0.);
        //FIXME: compare with arg bounds?

        std::stringstream ss;

        // Compute points
        std::string data_name = "data";

        // Argument (only 1D supported for now)
        typename GenericFunction<T>::vector_t x (f.inputSize ());
        x.setZero ();

        ss << data_name << " = np.array ([";

        for (value_type t = boost::get<0> (window); t < boost::get<1> (window);
             t += boost::get<2> (window))
	  {
	    // Evaluate the function
	    x[0] = t;
	    typename GenericFunction<T>::vector_t res = f (x);

	    ss << (boost::format ("(%2.8f") % normalize (t)).str ();

	    // Store the result in the vector of strings (for each output)
	    for (typename GenericFunction<T>::size_type i = 0; i < f.outputSize (); ++i)
	      {
		ss << (boost::format (", %2.8f") % normalize (res [i])).str ();
	      }
	    ss << "), ";
	  }
        ss << "])" << std::endl;

        // For each output dimension of the function
        if (f.outputSize () == 1)
	  {
	    ss << (boost::format ("plt.plot (%1%[:,0], %1%[:,1], label=\"%2%\")")
		   % data_name
		   % f.getName ()).str () << std::endl;
	  }
        else for (typename GenericFunction<T>::size_type i = 0;
                  i < f.outputSize (); ++i)
	       {
		 ss << (boost::format ("plt.plot (%1%[:,0], %1%[:,%2%], label=\"%3% (%4%)\")")
			% data_name
			% typename GenericFunction<T>::size_type (i+1)
			% f.getName ()
			% i).str () << std::endl;
	       }

        return Command (ss.str (), true);
      }

      /// @}
    } // end of namespace matplotlib.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_MATPLOTLIB_FUNCTION_HH
