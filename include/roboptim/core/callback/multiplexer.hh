// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_CALLBACK_MULTIPLEXER_HH
# define ROBOPTIM_CORE_CALLBACK_MULTIPLEXER_HH

# include <vector>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/solver-state.hh>

namespace roboptim
{
  namespace callback
  {

    /// \brief Callback multiplexer.
    ///
    /// This class turns multiple callback functions into one.
    /// Note that callbacks are processed in the order of the vector
    /// of functions provided. Beware of conflicts between multiple
    /// callbacks.
    ///
    /// \tparam S solver type.
    template <typename S>
    class Multiplexer
    {
    public:

      /// \brief Type of the solver.
      typedef S solver_t;

      /// \brief Type of the problem.
      typedef typename solver_t::problem_t problem_t;

      /// \brief Type of the state of the solver.
      typedef SolverState<problem_t> solverState_t;

      /// \brief Callback function type.
      typedef typename solver_t::callback_t callback_t;

      /// \brief Type of a vector of callbacks.
      typedef std::vector<callback_t> callbacks_t;

      /// \brief Default constructor containing no callback.
      /// \param solver solver the multiplexer will attach to.
      explicit Multiplexer (solver_t& solver);

      /// \brief Constructor filling the vector of callbacks.
      /// \param solver solver the multiplexer will attach to.
      /// \param callbacks a vector of callbacks.
      explicit Multiplexer (solver_t& solver, const callbacks_t& callbacks);

      /// \brief Virtual destructor.
      virtual ~Multiplexer ();

      /// \brief Return the vector of callbacks.
      /// \return vector of callbacks.
      callbacks_t& callbacks ();

      /// \brief Return the vector of callbacks.
      /// \return vector of callbacks.
      const callbacks_t& callbacks () const;

    protected:

      /// \brief Meta-callback calling multiple callbacks.
      ///
      /// \param pb problem.
      /// \param state solver state.
      void perIterationCallback (const problem_t& pb,
                                 solverState_t& state);

      /// \brief Meta-callback calling multiple callbacks.
      /// Unsafe version that can throw exceptions.
      ///
      /// \param pb problem.
      /// \param state solver state.
      virtual void perIterationCallbackUnsafe
      (const problem_t& pb,
       solverState_t& state);

      /// \brief Attach the multiplexer to a solver.
      /// \param solver solver that will use this multiplexer.
      void attach ();

    private:

      /// \brief Solver the multiplexer is bound to.
      solver_t& solver_;

      /// \brief Vector of callbacks.
      callbacks_t callbacks_;

    };

  } // end of namespace callback
} // end of namespace roboptim

# include <roboptim/core/callback/multiplexer.hxx>

#endif //! ROBOPTIM_CORE_CALLBACK_MULTIPLEXER_HH
