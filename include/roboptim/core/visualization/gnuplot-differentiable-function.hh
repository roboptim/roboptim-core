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
# include <boost/mpl/assert.hpp>
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

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
      /// \warning The sparse version currently relies on a sparse to dense
      /// matrix conversion. This can be highly inefficient for large matrices.
      ///
      /// \param f differentiable function whose Jacobian will be plotted
      /// \param arg optimization parameters of the point to plot
      /// \return Gnuplot command
      template <typename T>
      Command plot_jac (const GenericDifferentiableFunction<T>& f,
                        const argument_t& arg);

      template <>
      ROBOPTIM_DLLAPI
      Command plot_jac (const DifferentiableFunction& f,
			const argument_t& arg);
      template <>
      ROBOPTIM_DLLAPI
      Command plot_jac (const DifferentiableSparseFunction& f,
			const argument_t& arg);

      template <typename T>
      Command plot_jac (const GenericDifferentiableFunction<T>& f,
                        const argument_t& arg)
      {
	BOOST_MPL_ASSERT_MSG (false, NOT_IMPLEMENTED, ());
	return Command ("");
      }

      /// @}
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_DIFFERENTIABLE_FUNCTION_HH
