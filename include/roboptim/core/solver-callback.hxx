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

#ifndef ROBOPTIM_CORE_SOLVER_CALLBACK_HXX
# define ROBOPTIM_CORE_SOLVER_CALLBACK_HXX

# include <boost/bind.hpp>

namespace roboptim
{
  template <typename S>
  SolverCallback<S>::SolverCallback (const std::string& name)
    : name_ (name)
  {}

  template <typename S>
  SolverCallback<S>::~SolverCallback ()
  {}

  template <typename S>
  const std::string& SolverCallback<S>::name () const
  {
    return name_;
  }

  template <typename S>
  typename SolverCallback<S>::callback_t
  SolverCallback<S>::callback ()
  {
    return boost::bind (&SolverCallback<S>::perIterationCallback,
                        this, _1, _2);
  }


  template <typename S>
  void SolverCallback<S>::operator () (const problem_t& pb,
                                       solverState_t& state)
  {
    (callback ()) (pb, state);
  }

  template <typename S>
  void SolverCallback<S>::perIterationCallback
  (const problem_t& pb, solverState_t& state)
  {
    try
    {
      perIterationCallbackUnsafe (pb, state);
    }
    catch (std::exception& e)
    {
      std::cerr << e.what () << std::endl;
    }
    catch (...)
    {
      std::cerr << "unknown exception" << std::endl;
    }
  }

  template <typename S>
  std::ostream&
  SolverCallback<S>::print (std::ostream& o) const
  {
    o << name_;

    return o;
  }

  template <typename S>
  std::ostream&
  operator<< (std::ostream& o, const SolverCallback<S>& c)
  {
    return c.print (o);
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  extern template class SolverCallback<Solver<EigenMatrixDense> >;
  extern template class SolverCallback<Solver<EigenMatrixSparse> >;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_CALLBACK_HXX
