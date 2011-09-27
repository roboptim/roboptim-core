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

#include "debug.hh"

#include "roboptim/core/function.hh"
#include "roboptim/core/problem.hh"
#include "roboptim/core/plugin/dummy.hh"

namespace roboptim
{
  DummySolver::DummySolver (const problem_t& pb) throw ()
    : parent_t (pb)
  {
    parameters_["dummy-parameter"].description = "dummy parameter";
    parameters_["dummy-parameter"].value = 42.;

    parameters_["dummy-parameter2"].description = "yet another dummy parameter";
    parameters_["dummy-parameter2"].value = 3;

    parameters_["dummy-parameter3"].description = "just a dummy key";
    parameters_["dummy-parameter3"].value = "...and a dummy value!";
  }

  DummySolver::~DummySolver () throw ()
  {
  }

  void
  DummySolver::solve () throw ()
  {
    result_ = SolverError ("The dummy solver always fail.");
  }

} // end of namespace roboptim

extern "C"
{
  using namespace roboptim;
  typedef DummySolver::parent_t solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT solver_t* create (const DummySolver::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (solver_t::problem_t);
  }

  ROBOPTIM_DLLEXPORT solver_t* create (const DummySolver::problem_t& pb)
  {
    return new DummySolver (pb);
  }

  ROBOPTIM_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
