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

# include <boost/shared_ptr.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/portability.hh>
# include <roboptim/core/solver-state.hh>
# include <roboptim/core/solver-callback.hh>

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
    class ROBOPTIM_DLLAPI Multiplexer : public SolverCallback<S>
    {
    public:
      /// \brief Parent type.
      typedef SolverCallback<S> parent_t;

      /// \brief Type of the solver.
      typedef S solver_t;

      /// \brief Type of the problem.
      typedef typename solver_t::problem_t problem_t;

      /// \brief Type of the state of the solver.
      typedef SolverState<problem_t> solverState_t;

      /// \brief Solver callback type.
      typedef SolverCallback<S> solverCallback_t;

      /// \brief Pointer to a callback.
      typedef boost::shared_ptr<solverCallback_t> solverCallbackPtr_t;

      /// \brief Type of a vector of callbacks.
      typedef std::vector<solverCallbackPtr_t> solverCallbacks_t;

      /// \brief Default constructor containing no callback.
      /// \param solver solver the multiplexer will attach to.
      explicit Multiplexer (solver_t& solver);

      /// \brief Constructor filling the vector of callbacks.
      /// \param solver solver the multiplexer will attach to.
      /// \param callbacks a vector of callbacks.
      explicit Multiplexer (solver_t& solver,
                            const solverCallbacks_t& callbacks);

      /// \brief Virtual destructor.
      virtual ~Multiplexer ();

      /// \brief Return the vector of callbacks.
      /// \return vector of callbacks.
      solverCallbacks_t& callbacks ();

      /// \brief Return the vector of callbacks.
      /// \return vector of callbacks.
      const solverCallbacks_t& callbacks () const;

      /// \brief Display the callback on the specified output stream.
      /// \param o output stream used for display.
      /// \return output stream.
      virtual std::ostream& print (std::ostream& o) const;

    protected:

      /// \brief Meta-callback calling multiple callbacks.
      /// Unsafe version that can throw exceptions.
      ///
      /// \param pb problem.
      /// \param state solver state.
      virtual void perIterationCallbackUnsafe
      (const problem_t& pb, solverState_t& state);

      /// \brief Register the multiplexer with the solver.
      void attach ();

      /// \brief Unregister the multiplexer from the solver.
      void unregister ();

    private:

      /// \brief Solver the multiplexer is bound to.
      solver_t& solver_;

      /// \brief Vector of callbacks.
      solverCallbacks_t callbacks_;
    };
  } // end of namespace callback
} // end of namespace roboptim

# include <roboptim/core/callback/multiplexer.hxx>

#endif //! ROBOPTIM_CORE_CALLBACK_MULTIPLEXER_HH
