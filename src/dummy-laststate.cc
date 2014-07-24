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

#include <typeinfo>

#include "roboptim/core/function.hh"
#include "roboptim/core/problem.hh"
#include "roboptim/core/plugin/dummy-laststate.hh"

namespace roboptim
{
  DummySolverLastState::DummySolverLastState (const problem_t& pb)
    : parent_t (pb),
      callback_ (),
      solverState_ (pb)
  {
    parameters_["dummy-parameter"].description = "dummy parameter";
    parameters_["dummy-parameter"].value = 42.;

    parameters_["dummy-parameter2"].description = "yet another dummy parameter";
    parameters_["dummy-parameter2"].value = 3;

    parameters_["dummy-parameter3"].description = "just a dummy key";
    parameters_["dummy-parameter3"].value = "...and a dummy value!";
  }

  DummySolverLastState::~DummySolverLastState ()
  {
  }

  void
  DummySolverLastState::solve ()
  {
    // Set some dummy values for the last state of the solver
    Result res (1, 1);
    res.x.fill(1337);
    res.constraints.fill(0);
    res.lambda.fill(0);
    res.value.fill(42);


    // Also fill solver state (for logger testing)
    solverState_.x () = res.x;
    solverState_.cost () = res.value[0];
    solverState_.constraintViolation () = 42;

    // call user-defined callback
    if (callback_)
      callback_ (problem (), solverState_);

    // Add these values to SolverError
    result_ = SolverError ("The dummy solver always fail.", res);
  }

} // end of namespace roboptim

extern "C"
{
  using namespace roboptim;
  typedef DummySolverLastState::parent_t solver_t;

  ROBOPTIM_DLLEXPORT std::size_t getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ();
  ROBOPTIM_DLLEXPORT solver_t* create
  (const DummySolverLastState::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT std::size_t getSizeOfProblem ()
  {
    return sizeof (solver_t::problem_t);
  }

  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ()
  {
    return typeid (solver_t::problem_t::constraintsList_t).name ();
  }

  ROBOPTIM_DLLEXPORT solver_t* create
  (const DummySolverLastState::problem_t& pb)
  {
    return new DummySolverLastState (pb);
  }

  ROBOPTIM_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
