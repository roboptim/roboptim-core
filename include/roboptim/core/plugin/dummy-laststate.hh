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


#ifndef ROBOPTIM_CORE_DUMMY_LASTSTATE_HH
# define ROBOPTIM_CORE_DUMMY_LASTSTATE_HH
# include <boost/mpl/vector.hpp>

# include <roboptim/core/solver.hh>
# include <roboptim/core/solver-state.hh>

namespace roboptim
{
  /// \brief Dummy solver which always fails, but returns the last state of the
  /// solver.
  ///
  /// The SolverError generated contains a dummy state of the solver (x,
  /// constraints etc.). These values can be obtained thanks to
  /// SolverError::lastState.
  ///
  /// This solver always fails but is always available
  /// as it does not rely on the plug-in mechanism.
  ///
  /// It is also a good starting point for users that
  /// want to develop their own solver.
  template <typename T>
  class GenericDummySolverLastState
    : public Solver<T>
  {
  public:
    /// \brief Define parent's type.
    typedef Solver<T> parent_t;

    /// \brief Problem type.
    typedef typename parent_t::problem_t problem_t;

    /// \brief Callback function type.
    typedef typename parent_t::callback_t callback_t;

    /// \brief Type of the state of the solver.
    typedef SolverState<problem_t> solverState_t;

    /// \brief Build a solver from a problem.
    /// \param problem problem that will be solved
    explicit GenericDummySolverLastState (const problem_t& problem);

    virtual ~GenericDummySolverLastState ();

    /// \brief Implement the solve algorithm.
    ///
    /// Implement the solve method as required by the
    /// #GenericSolver class.
    virtual void solve ();

    virtual void setIterationCallback (callback_t callback)
    {
      callback_ = callback;
    }

    const callback_t& callback () const
    {
      return callback_;
    }

    /// \brief Intermediate callback (called at each end of iteration).
    callback_t callback_;

    /// \brief Current state of the solver (used by the callback function).
    solverState_t solverState_;
  };

  typedef GenericDummySolverLastState<EigenMatrixDense> DummySolverLastState;
  typedef GenericDummySolverLastState<EigenMatrixSparse>
  DummyDifferentiableSparseSolverLastState;

  // Explicit instantiation
  template class GenericDummySolverLastState<EigenMatrixDense>;
  template class GenericDummySolverLastState<EigenMatrixSparse>;

} // end of namespace roboptim

# include <roboptim/core/plugin/dummy-laststate.hxx>

#endif //! ROBOPTIM_CORE_DUMMY_LASTSTATE_HH
