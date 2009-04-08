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
 * \brief Declaration of the Problem class.
 */

#ifndef OPTIMIZATION_PROBLEM_HH
# define OPTIMIZATION_PROBLEM_HH
# include <boost/optional.hpp>
# include <liboptimization/fwd.hh>
# include <liboptimization/function.hh>

namespace optimization
{
  template <typename F, typename C>
  class Problem
  {
  public:
    typedef F function_t;
    typedef C constraint_t;

    typedef std::vector<const constraint_t*> constraints_t;
    typedef boost::optional<Function::vector_t> startingPoint_t;

    explicit Problem (const function_t&) throw ();
    ~Problem () throw ();

    const function_t& getFunction () const throw ();

    const constraints_t& getConstraints () const throw ();
    constraints_t& getConstraints () throw ();

    startingPoint_t& getStartingPoint () throw ();
    const startingPoint_t& getStartingPoint () const throw ();

  private:
    const function_t& function_;
    startingPoint_t startingPoint_;
    constraints_t constraints_;
  };
} // end of namespace optimization

# include <liboptimization/problem.hxx>
#endif //! OPTIMIZATION_PROBLEM_HH
