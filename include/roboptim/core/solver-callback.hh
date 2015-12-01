// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_SOLVER_CALLBACK_HH
# define ROBOPTIM_CORE_SOLVER_CALLBACK_HH

# include <string>
# include <ostream>

# include <boost/function.hpp>

# include <roboptim/core/solver-state.hh>

namespace roboptim
{
  /// \brief Solver per-iteration callback wrapper.
  /// \tparam S solver type.
  template <typename S>
  class SolverCallback
  {
  public:
    /// \brief Solver type.
    typedef S solver_t;

    /// \brief Solver problem type.
    typedef typename solver_t::problem_t problem_t;

    /// Per-iteration callback type
    ///
    /// Callback parameters:
    /// \li problem is a (constant) reference to the problem
    /// \li state is the current state of the optimization solver. It can be
    ///     modified by the callback, and updated values can be used by the
    ///     solver to determine what to do next.
    typedef typename solver_t::callback_t callback_t;

    /// \brief State of the solver.
    typedef typename solver_t::solverState_t solverState_t;

  public:
    /// \brief Solver callback constructor.
    /// \param name name of the callback.
    SolverCallback (const std::string& name = "unnamed callback");

    /// \brief Destructor.
    virtual ~SolverCallback ();

    /// \brief Return the name of the callback.
    /// \return name of the callback.
    const std::string& name () const;

    /// \brief Callback function.
    callback_t callback ();

    /// \brief Evaluate the callback.
    void operator () (const problem_t& pb, solverState_t& state);

    /// \brief Display the callback on the specified output stream.
    /// \param o output stream used for display.
    /// \return output stream.
    virtual std::ostream& print (std::ostream& o) const;

  protected:
    /// \brief Wrapper around the callback function that catch exceptions.
    /// \param pb optimization problem.
    /// \param state state of the solver after an iteration.
    virtual void perIterationCallback
      (const problem_t& pb, solverState_t& state);

    /// \brief The actual callback function.
    /// The function may throw.
    /// \param pb optimization problem.
    /// \param state state of the solver after an iteration.
    virtual void perIterationCallbackUnsafe
      (const problem_t& pb, solverState_t& state) = 0;

  private:
    /// \brief Name of the callback.
    std::string name_;
  };

  template <typename S>
  std::ostream&
  operator<< (std::ostream& o, const SolverCallback<S>& c);
} // end of namespace roboptim.

# include <roboptim/core/solver-callback.hxx>

#endif //! ROBOPTIM_CORE_SOLVER_CALLBACK_HH
