// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
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

namespace roboptim
{

  template <typename F>
  GenericDummySolverLastState<F>::GenericDummySolverLastState
  (const problem_t& pb)
    : parent_t (pb),
      callback_ (),
      solverState_ (pb)
  {
    parent_t::parameters_["dummy-parameter"].description = "dummy parameter";
    parent_t::parameters_["dummy-parameter"].value = 42.;

    parent_t::parameters_["dummy-parameter2"].description = "yet another dummy parameter";
    parent_t::parameters_["dummy-parameter2"].value = 3;

    parent_t::parameters_["dummy-parameter3"].description = "just a dummy key";
    parent_t::parameters_["dummy-parameter3"].value = "...and a dummy value!";
  }

  template <typename F>
  GenericDummySolverLastState<F>::~GenericDummySolverLastState ()
  {
  }

  template <typename F>
  void GenericDummySolverLastState<F>::solve ()
  {
    // Set some dummy values for the last state of the solver
    Result res (parent_t::problem ().function ().inputSize (),
                parent_t::problem ().function ().outputSize ());
    res.x.fill (1337);
    res.constraints.fill (0);
    res.lambda.fill (0);
    res.value.fill (42);

    // Also fill solver state (for logger testing)
    solverState_.x () = res.x;
    solverState_.cost () = res.value[0];
    solverState_.constraintViolation () = 42;

    // call user-defined callback
    if (callback_)
      callback_ (parent_t::problem (), solverState_);

    // Add these values to SolverError
    parent_t::result_ = SolverError ("The dummy solver always fail.", res);
  }

} // end of namespace roboptim
