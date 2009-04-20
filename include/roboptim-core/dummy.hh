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


/**
 * \brief Implementation of the dummy module (always fail).
 */

#ifndef ROBOPTIM_CORE_DUMMY_HH
# define ROBOPTIM_CORE_DUMMY_HH
# include <roboptim-core/solver.hh>

namespace roboptim
{
  /// Dummy solver which always fails.
  class DummySolver : public Solver<Function, const Function*>
  {
  public:
    /// Define parent's type.
    typedef Solver<Function, const Function*> parent_t;

    /// Constructor.
    explicit DummySolver (const problem_t&) throw ();
    /// Destructor.
    virtual ~DummySolver () throw ();

    /// Implement the solve algorithm.
    virtual void solve () throw ();
  };

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DUMMY_HH
