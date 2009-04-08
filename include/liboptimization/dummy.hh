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
 * \brief Implementation of the dummy module (always fail).
 */

#ifndef OPTIMIZATION_DUMMY_HH
# define OPTIMIZATION_DUMMY_HH
# include <liboptimization/solver.hh>

namespace optimization
{
  /// Dummy solver which always fails.
  class DummySolver : public Solver<Function, Function>
  {
  public:
    /// Define parent's type.
    typedef Solver<Function, Function> parent_t;

    /// Constructor.
    explicit DummySolver (const problem_t&) throw ();
    /// Destructor.
    virtual ~DummySolver () throw ();

    /// Implement the solve algorithm.
    virtual void solve () throw ();
  };

}; // end of namespace optimization

#endif //! OPTIMIZATION_DUMMY_HH
