// Copyright (C) 2013 by Benjamin Chretien, AIST, CNRS.
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

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_MATRIX_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_MATRIX_HH
# include <boost/mpl/assert.hpp>
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <roboptim/core/function.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      /// \addtogroup roboptim_visualization
      /// @{

      /// \brief Plot the structure of a matrix with Gnuplot.
      ///
      /// Plot the structure of a matrix with Gnuplot. Nonzero values will be
      /// displayed in blue, zeros in white.
      ///
      /// When dealing with a dense matrix, the actual values are printed and
      /// exact zeros will be displayed in white.
      ///
      /// When dealing with a sparse matrix, inserted zeros will also be treated
      /// as nonzero values, thus returning the real structure of the sparse
      /// matrix. In this case, the values returned are 1 for sparse elements,
      /// and 0 for actual zeros.
      ///
      /// \param mat matrix to plot.
      /// \return Gnuplot command.

      ROBOPTIM_DLLAPI
      Command plot_mat
      (const GenericFunctionTraits<EigenMatrixDense>::matrix_t& mat);

      ROBOPTIM_DLLAPI
      Command plot_mat
      (const GenericFunctionTraits<EigenMatrixSparse>::matrix_t& mat);

      template <typename T>
      Command plot_mat
      (const typename GenericFunctionTraits<T>::matrix_t&)
      {
        BOOST_MPL_ASSERT_MSG (false, NOT_IMPLEMENTED, ());
        return Command ("");
      }

      /// @}
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_MATRIX_HH
