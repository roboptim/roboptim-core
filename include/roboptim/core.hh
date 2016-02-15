// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_HH
# define ROBOPTIM_CORE_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

// Generic headers.
# include <roboptim/core/fwd.hh>
# include <roboptim/core/io.hh>
# include <roboptim/core/visualization/fwd.hh>


// Main headers.
# include <roboptim/core/cache.hh>
# include <roboptim/core/indent.hh>
# include <roboptim/core/terminal-color.hh>
# include <roboptim/core/util.hh>

# include <roboptim/core/function.hh>
# include <roboptim/core/differentiable-function.hh>
# include <roboptim/core/derivable-parametrized-function.hh>
# include <roboptim/core/linear-function.hh>
# include <roboptim/core/n-times-derivable-function.hh>
# include <roboptim/core/numeric-linear-function.hh>
# include <roboptim/core/numeric-quadratic-function.hh>
# include <roboptim/core/parametrized-function.hh>
# include <roboptim/core/quadratic-function.hh>
# include <roboptim/core/sum-of-c1-squares.hh>
# include <roboptim/core/twice-differentiable-function.hh>

# include <roboptim/core/problem.hh>
# include <roboptim/core/generic-solver.hh>
# include <roboptim/core/solver.hh>
# include <roboptim/core/solver-callback.hh>
# include <roboptim/core/solver-error.hh>
# include <roboptim/core/solver-factory.hh>
# include <roboptim/core/solver-state.hh>
# include <roboptim/core/solver-warning.hh>

# include <roboptim/core/result-with-warnings.hh>
# include <roboptim/core/result.hh>

# include <roboptim/core/optimization-logger.hh>
# include <roboptim/core/scaling-helper.hh>
# include <roboptim/core/derivative-size.hh>


// Callbacks.
# include <roboptim/core/callback/wrapper.hh>
# include <roboptim/core/callback/multiplexer.hh>

// Decorators.
# include <roboptim/core/decorator/cached-function.hh>
# include <roboptim/core/decorator/finite-difference-gradient.hh>

// Operators.
# include <roboptim/core/operator/bind.hh>
# include <roboptim/core/operator/chain.hh>
# include <roboptim/core/operator/concatenate.hh>
# include <roboptim/core/operator/derivative.hh>
# include <roboptim/core/operator/map.hh>
# include <roboptim/core/operator/minus.hh>
# include <roboptim/core/operator/plus.hh>
# include <roboptim/core/operator/product.hh>
# include <roboptim/core/operator/scalar.hh>
# include <roboptim/core/operator/selection.hh>
# include <roboptim/core/operator/selection-by-id.hh>
# include <roboptim/core/operator/split.hh>


// Functions.
# include <roboptim/core/function/constant.hh>
# include <roboptim/core/function/cos.hh>
# include <roboptim/core/function/identity.hh>
# include <roboptim/core/function/polynomial.hh>
# include <roboptim/core/function/sin.hh>


// Visualization.
# include <roboptim/core/visualization/gnuplot.hh>
# include <roboptim/core/visualization/gnuplot-commands.hh>
# include <roboptim/core/visualization/gnuplot-differentiable-function.hh>
# include <roboptim/core/visualization/gnuplot-function.hh>
# include <roboptim/core/visualization/gnuplot-matrix.hh>
# include <roboptim/core/visualization/matplotlib.hh>
# include <roboptim/core/visualization/matplotlib-commands.hh>
# include <roboptim/core/visualization/matplotlib-function.hh>
# include <roboptim/core/visualization/matplotlib-matrix.hh>


#endif //! ROBOPTIM_CORE_HH
