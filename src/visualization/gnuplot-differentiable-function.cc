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

      std::string dense_jacobian_to_gnuplot
      (GenericFunctionTraits<EigenMatrixDense>::matrix_t jac,
       std::string name)
      {
        typedef GenericFunctionTraits<EigenMatrixDense>::matrix_t matrix_t;

        std::string str = "";

        // Title of the graph
        str += "set title 'jacobian(" + name + ")'\n";

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

        for (matrix_t::Index cstr_id = 0;
             cstr_id < jac.rows(); ++cstr_id)
          for (matrix_t::Index out_id = 0;
               out_id < jac.cols(); ++out_id)
            {
              str += (boost::format("%1.2f")
		      % normalize(jac(cstr_id, out_id))).str();

              if (out_id < jac.cols() - 1) str += " ";
              else str += "\n";
            }
        str += "e\n";
        return str;
      }

      template <>
      Command plot_jac (const DifferentiableFunction& f,
			const argument_t& arg)
      {
	DifferentiableFunction::jacobian_t jac = f.jacobian(arg);

	return Command (dense_jacobian_to_gnuplot(jac, f.getName()));
      }


      template <>
      Command plot_jac (const DifferentiableSparseFunction& f,
			const argument_t& arg)
      {
	// Note: Gnuplot does not support sparse matrix display (yet).
	//       We currently convert the sparse matrix to a dense matrix,
	//       which is highly inefficient for large matrices.
	GenericFunctionTraits<EigenMatrixDense>::matrix_t
	  jac_dense = sparse_to_dense(f.jacobian(arg));

	return Command (dense_jacobian_to_gnuplot(jac_dense, f.getName()));
      }

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim
