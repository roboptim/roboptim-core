// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.

/**
 * \file src/problem.hh
 *
 * \brief Declaration of the Problem class.
 */

#ifndef OPTIMIZATION_PROBLEM_HH
# define OPTIMIZATION_PROBLEM_HH
# include <vector>
# include <boost/shared_ptr.hpp>

# include <optimization-fwd.hh>
# include <function.hh>

namespace optimization
{
  /// Define an optimization problem.
  struct Problem
  {
    typedef boost::shared_ptr<Function> functionPtr_t;
    typedef std::vector<functionPtr_t> constraints_t;

    Problem (const Function&) throw ();
    ~Problem () throw ();

    const Function& function;
    constraints_t constraints;

    /// Starting point.
    Function::vector_t start;
  };
} // end of namespace optimization

#endif //! OPTIMIZATION_PROBLEM_HH
