// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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

#include "debug.hh"

#include <typeinfo>

#include "roboptim/core/function.hh"
#include "roboptim/core/plugin/dummy.hh"

extern "C"
{
  typedef roboptim::GenericDummySolver<roboptim::EigenMatrixSparse> DummySparseSolver;
  typedef DummySparseSolver::parent_t solver_t;

  ROBOPTIM_CORE_DLLEXPORT std::size_t getSizeOfProblem ();
  ROBOPTIM_CORE_DLLEXPORT const char* getTypeIdOfConstraintsList ();
  ROBOPTIM_CORE_DLLEXPORT solver_t* create (const DummySparseSolver::problem_t& pb);
  ROBOPTIM_CORE_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_CORE_DLLEXPORT std::size_t getSizeOfProblem ()
  {
    return sizeof (solver_t::problem_t);
  }

  ROBOPTIM_CORE_DLLEXPORT const char* getTypeIdOfConstraintsList ()
  {
    return typeid (solver_t::problem_t::constraintsList_t).name ();
  }

  ROBOPTIM_CORE_DLLEXPORT solver_t* create (const DummySparseSolver::problem_t& pb)
  {
    return new DummySparseSolver (pb);
  }

  ROBOPTIM_CORE_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
