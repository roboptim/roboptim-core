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

      /// \brief Plot the Jacobian structure with Gnuplot.
      ///
      /// Plot the structure of the Jacobian of a differentiable function with
      /// Gnuplot. Nonzero values will be displayed in blue, zeros in white.
      ///
      /// When dealing with a dense matrix, the actual values are printed and
      /// exact zeros will be displayed in white.
      ///
      /// When dealing with a sparse matrix, inserted zeros will also be treated
      /// as nonzero values, thus returning the real structure of the sparse
      /// Jacobian matrix. In this case, the values returned are 1 for sparse
      /// elements, and 0 for actual zeros.
      ///
      /// \param f differentiable function whose Jacobian will be plotted
      /// \param arg optimization parameters of the point to plot. This
      /// parameter is not relevant when dealing with sparse functions, since
      /// it should not change the Jacobian's sparse structure.
      /// \return Gnuplot command
      template <typename T>
      Command plot_jac (const GenericDifferentiableFunction<T>& f,
                        typename GenericDifferentiableFunction<T>::
                        const_argument_ref arg);

      template <>
      ROBOPTIM_CORE_DLLAPI
      Command plot_jac (const DifferentiableFunction& f,
                        DifferentiableFunction::const_argument_ref arg);
      template <>
      ROBOPTIM_CORE_DLLAPI
      Command plot_jac (const DifferentiableSparseFunction& f,
                        DifferentiableSparseFunction::const_argument_ref arg);

      template <typename T>
      Command plot_jac (const GenericDifferentiableFunction<T>&,
                        typename GenericDifferentiableFunction<T>::
                        const_argument_ref)
      {
	BOOST_MPL_ASSERT_MSG (false, NOT_IMPLEMENTED, ());
	return Command ("");
      }

      /// @}
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_DIFFERENTIABLE_FUNCTION_HH
