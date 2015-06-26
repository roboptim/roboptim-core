// Copyright (C) 2013 by Benjamin Chretien.
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


#ifndef ROBOPTIM_CORE_DUMMY_TD_HH
# define ROBOPTIM_CORE_DUMMY_TD_HH
# include <boost/mpl/vector.hpp>

# include <roboptim/core/solver.hh>

namespace roboptim
{
  /// \brief Dummy solver which always fails.
  ///
  /// This solver always fails but is always available
  /// as it does not rely on the plug-in mechanism.
  ///
  /// It is also a good starting point for users that
  /// want to develop their own solver.
  class DummySolverTd : public Solver<TwiceDifferentiableFunction>
  {
  public:
    /// \brief Define parent's type.
    typedef Solver<TwiceDifferentiableFunction> parent_t;

    /// \brief Build a solver from a problem.
    /// \param problem problem that will be solved
    explicit DummySolverTd (const problem_t& problem);

    virtual ~DummySolverTd ();

    /// \brief Implement the solve algorithm.
    ///
    /// Implement the solve method as required by the
    /// #GenericSolver class.
    virtual void solve ();
  };

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DUMMY_TD_HH
