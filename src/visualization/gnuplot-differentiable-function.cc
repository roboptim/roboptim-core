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
#include <roboptim/core/visualization/gnuplot-matrix.hh>
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

        std::string dense_jacobian_to_gnuplot
        (const DifferentiableFunction::jacobian_t& jac,
         const std::string& name)
        {
          std::string str = "set title 'Dense Jacobian (" + name + ")'\n";
          str += plot_mat (jac).command ();

          return str;
        }


        std::string sparse_jacobian_to_gnuplot
        (const DifferentiableSparseFunction::jacobian_t& jac,
         const std::string& name)
        {
          std::string str = "set title 'Sparse Jacobian (" + name + ")'\n";
          str += plot_mat (jac).command ();

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
