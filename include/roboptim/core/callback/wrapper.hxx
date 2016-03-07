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

#ifndef ROBOPTIM_CORE_CALLBACK_WRAPPER_HXX
# define ROBOPTIM_CORE_CALLBACK_WRAPPER_HXX

# include <roboptim/core/solver.hh>
# include <roboptim/core/solver-state.hh>
# include <roboptim/core/solver-callback.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  namespace callback
  {
    template <typename S>
    Wrapper<S>::Wrapper (callback_t c, const std::string& name)
    : parent_t (name),
      callback_ (c)
    {
    }

    template <typename S>
    Wrapper<S>::~Wrapper ()
    {}

    template <typename S>
    void Wrapper<S>::perIterationCallbackUnsafe
    (const problem_t& pb, solverState_t& state)
    {
      (callback_) (pb, state);
    }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
    extern template class ROBOPTIM_CORE_DLLAPI Wrapper<Solver<EigenMatrixDense> >;
    extern template class ROBOPTIM_CORE_DLLAPI Wrapper<Solver<EigenMatrixSparse> >;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

  } // end of namespace callback
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_CALLBACK_WRAPPER_HXX
