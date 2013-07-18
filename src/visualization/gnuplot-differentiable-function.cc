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

#include <roboptim/core/visualization/gnuplot.hh>
#include <roboptim/core/visualization/gnuplot-commands.hh>
#include <roboptim/core/visualization/gnuplot-differentiable-function.hh>

# include <boost/format.hpp>
# include <boost/lexical_cast.hpp>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      namespace detail
      {

        template <typename T>
        void set_jacobian_header
        (std::string& str,
         const typename GenericFunctionTraits<T>::jacobian_t& jac)
        {
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
        }

        std::string dense_jacobian_to_gnuplot
        (const DifferentiableFunction::jacobian_t& jac,
         const std::string& name)
        {
          typedef DifferentiableFunction::jacobian_t jacobian_t;

          std::string str = "";

          // Set the header of the Gnuplot output (title, range, etc.)
          str += "set title 'Dense Jacobian (" + name + ")'\n";
          set_jacobian_header<EigenMatrixDense>(str, jac);

          for (jacobian_t::Index cstr_id = 0;
               cstr_id < jac.rows(); ++cstr_id)
            for (jacobian_t::Index out_id = 0;
                 out_id < jac.cols(); ++out_id)
              {
                str += (boost::format("%2.8f")
                        % normalize(jac(cstr_id, out_id))).str();

                if (out_id < jac.cols() - 1) str += " ";
                else str += "\n";
              }
          str += "e\n";
          str += "e\n";
          return str;
        }

        std::string sparse_jacobian_to_gnuplot
        (const DifferentiableSparseFunction::jacobian_t& jac,
         const std::string& name)
        {
          typedef DifferentiableSparseFunction::jacobian_t jacobian_t;

          std::string str = "";

          // Set the header of the Gnuplot output (range, etc.)
          str += "set title 'Sparse Jacobian (" + name + ")'\n";
          set_jacobian_header<EigenMatrixSparse>(str, jac);

          // Since Gnuplot does not support sparse matrices, we will need to
          // plot all the zeros of the sparse matrices.
          // Outer dimension
          for (int k = 0; k < jac.outerSize(); ++k)
            {
              // Inner dimension
              jacobian_t::InnerIterator it(jac,k);
              for (int col_it = 0; col_it < jac.cols (); ++col_it)
                {
                  // Sparse nonzero: return 1
                  if (col_it == it.col ())
                    {
                      str += "1";
                      ++it;
                    }
                  // Sparse zero: return 0
                  else str += "0";

                  if (col_it < jac.cols() - 1) str += " ";
                  else str += "\n";
                }
            }
          str += "e\n";
          str += "e\n";
          return str;
        }

      } // end of namespace detail.

      template <>
      Command plot_jac (const DifferentiableFunction& f,
                        const DifferentiableFunction::argument_t& arg)
      {
	DifferentiableFunction::jacobian_t jac = f.jacobian(arg);

        return Command (detail::dense_jacobian_to_gnuplot
                        (jac, f.getName()));
      }


      template <>
      Command plot_jac (const DifferentiableSparseFunction& f,
                        const DifferentiableSparseFunction::argument_t& arg)
      {
        DifferentiableSparseFunction::jacobian_t jac = f.jacobian(arg);

        return Command (detail::sparse_jacobian_to_gnuplot
                        (jac, f.getName()));
      }

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim
