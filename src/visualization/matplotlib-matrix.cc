// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

#include <sstream>

#include <roboptim/core/visualization/matplotlib.hh>
#include <roboptim/core/visualization/matplotlib-commands.hh>
#include <roboptim/core/visualization/matplotlib-matrix.hh>

# include <boost/format.hpp>
# include <boost/lexical_cast.hpp>

namespace roboptim
{
  namespace visualization
  {
    namespace matplotlib
    {
      namespace detail
      {

        template <typename T>
        void set_matrix_header
        (std::string& str,
         typename GenericFunctionTraits<T>::const_matrix_ref mat)
        {
          assert (0);
        }

        std::string dense_matrix_to_matplotlib
        (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref mat)
        {
          typedef GenericFunctionTraits<EigenMatrixDense>::matrix_t matrix_t;

          std::stringstream ss;

          ss << "data = np.array ([";

          for (matrix_t::Index cstr_id = 0;
               cstr_id < mat.rows (); ++cstr_id)
          {
            ss << "[" << std::endl;
            for (matrix_t::Index out_id = 0;
                 out_id < mat.cols (); ++out_id)
              {
                ss << (boost::format("%2.8f")
                       % normalize (mat (cstr_id, out_id))).str();

                if (out_id < mat.cols() - 1) ss << ",";
                else ss << "],\n";
              }
          }

          ss << "])" << std::endl;

          std::string cmap = "matplotlib.colors.ListedColormap(['white', 'blue'])";
          ss << "plt.imshow(data, interpolation='nearest', cmap="
             << cmap << ")" << std::endl;

          return ss.str ();
        }

        std::string sparse_matrix_to_matplotlib
        (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref mat)
        {
          typedef GenericFunctionTraits<EigenMatrixSparse>::matrix_t matrix_t;

          std::stringstream ss;

          ss << "data = np.zeros ((" << mat.rows () << ", " << mat.cols () << "))\n";

          for (matrix_t::Index k = 0; k < mat.outerSize (); ++k)
          {
            for (matrix_t::InnerIterator it (mat, k); it; ++it)
              {
                assert ((StorageOrder == Eigen::ColMajor && it.row () == it.index ())
                        ||
                        (StorageOrder == Eigen::RowMajor && it.col () == it.index ()));

                ss << (boost::format("data[%i,%i] = %2.8f\n")
                       % it.row () % it.col () % normalize (it.value ())).str();
              }
          }

          std::string cmap = "matplotlib.colors.ListedColormap(['white', 'blue'])";
          ss << "plt.imshow(data, interpolation='nearest', cmap="
             << cmap << ")" << std::endl;

          return ss.str ();
        }

      } // end of namespace detail.


      Command plot_mat
      (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref mat)
      {
        return Command (detail::dense_matrix_to_matplotlib (mat), true);
      }


      Command plot_mat
      (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref mat)
      {
        return Command (detail::sparse_matrix_to_matplotlib (mat), true);
      }

    } // end of namespace matplotlib.
  } // end of namespace visualization.
} // end of namespace roboptim
