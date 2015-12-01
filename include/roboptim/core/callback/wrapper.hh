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

#ifndef ROBOPTIM_CORE_CALLBACK_WRAPPER_HH
# define ROBOPTIM_CORE_CALLBACK_WRAPPER_HH

# include <roboptim/core/solver-state.hh>
# include <roboptim/core/solver-callback.hh>

namespace roboptim
{
  namespace callback
  {
    /// \brief Raw callback wrapper.
    /// \tparam S solver type.
    template <typename S>
    class ROBOPTIM_DLLAPI Wrapper : public SolverCallback<S>
    {
    public:
      /// \brief Parent type.
      typedef SolverCallback<S> parent_t;

      /// \brief Type of the solver.
      typedef S solver_t;

      /// \brief Callback function type.
      typedef typename solver_t::callback_t callback_t;

      /// \brief Problem type.
      typedef typename solver_t::problem_t problem_t;

      /// \brief Type of the state of the solver.
      typedef SolverState<problem_t> solverState_t;

      /// \brief Default constructor.
      /// \param c raw callback function.
      /// \param name optional name.
      Wrapper (callback_t c, const std::string& name = "wrapped callback");

      /// \brief Virtual destructor.
      virtual ~Wrapper ();

    protected:

      /// \brief Meta-callback calling multiple callbacks.
      /// Unsafe version that can throw exceptions.
      ///
      /// \param pb problem.
      /// \param state solver state.
      virtual void perIterationCallbackUnsafe
      (const problem_t& pb, solverState_t& state);

    private:
      /// \brief Callback function.
      callback_t callback_;
    };
  } // end of namespace callback
} // end of namespace roboptim

# include <roboptim/core/callback/wrapper.hxx>

#endif //! ROBOPTIM_CORE_CALLBACK_WRAPPER_HH
