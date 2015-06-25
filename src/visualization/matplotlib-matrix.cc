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
        std::string set_red_to_blue_cmap ()
        {
          std::stringstream ss;

          ss << "cmap = matplotlib.colors.LinearSegmentedColormap.from_list(\n"
             << "         name='red_white_blue',\n"
             << "         colors =[(0, 0, 1),\n"
             << "                  (1, 1., 1),\n"
             << "                  (1, 0, 0)])\n";

          return ss.str ();
        }

        std::string dense_matrix_to_matplotlib
        (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref mat,
         bool structureOnly)
        {
          typedef GenericFunctionTraits<EigenMatrixDense>::matrix_t matrix_t;

          std::stringstream ss;

          ss << "data = np.array ([";

          for (matrix_t::Index cstr_id = 0;
               cstr_id < mat.rows (); ++cstr_id)
          {
            ss << "[";
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

          if (structureOnly)
          {
            std::string cmap = "matplotlib.colors.ListedColormap(['white', 'blue'])";
            ss << "plt.imshow(data > 0, interpolation='nearest', cmap=" << cmap << ")\n";
          }
          else
          {
            std::string set_cmap = set_red_to_blue_cmap ();
            ss << set_cmap
               << "crange = abs(data).max()\n"
               << "im = plt.imshow(data, interpolation='nearest', cmap=cmap, vmin=-crange, vmax=crange)\n"
               << "cbar = plt.colorbar(im)\n"
               << "cbar.draw_all()\n";
          }

          return ss.str ();
        }

        std::string sparse_matrix_to_matplotlib
        (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref mat,
         bool structureOnly)
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

                double value = 1.;
                if (!structureOnly)
                  value = normalize (it.value ());

                ss << (boost::format("data[%i,%i] = %2.8f\n")
                       % it.row () % it.col () % value).str();
              }
          }

          if (structureOnly)
          {
            std::string cmap = "matplotlib.colors.ListedColormap(['white', 'blue'])";
            ss << "plt.imshow(data, interpolation='nearest', cmap=" << cmap << ")" << std::endl;
          }
          else
          {
            std::string set_cmap = set_red_to_blue_cmap ();
            ss << set_cmap
               << "crange = abs(data).max()\n"
               << "im = plt.imshow(data, interpolation='nearest', cmap=cmap, vmin=-crange, vmax=crange)\n"
               << "cbar = plt.colorbar(im)\n"
               << "cbar.draw_all()\n";
          }

          return ss.str ();
        }

      } // end of namespace detail.


      Command plot_mat
      (GenericFunctionTraits<EigenMatrixDense>::const_matrix_ref mat,
       bool structureOnly)
      {
        return Command (detail::dense_matrix_to_matplotlib (mat, structureOnly), true);
      }


      Command plot_mat
      (GenericFunctionTraits<EigenMatrixSparse>::const_matrix_ref mat,
       bool structureOnly)
      {
        return Command (detail::sparse_matrix_to_matplotlib (mat, structureOnly), true);
      }

    } // end of namespace matplotlib.
  } // end of namespace visualization.
} // end of namespace roboptim
