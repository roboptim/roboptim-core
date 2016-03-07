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
#include "roboptim/core/plugin/dummy.hh"

namespace roboptim
{
  DummySolver::DummySolver (const problem_t& pb)
    : parent_t (pb)
  {
    parameters_["dummy-parameter"].description = "dummy parameter";
    parameters_["dummy-parameter"].value = 42.;

    parameters_["dummy-parameter2"].description = "yet another dummy parameter";
    parameters_["dummy-parameter2"].value = 3;

    parameters_["dummy-parameter3"].description = "just a dummy key";
    parameters_["dummy-parameter3"].value = std::string ("...and a dummy value!");

    Parameter::vector_t v (4);
    v << 1., 2., 3., 4.;
    parameters_["dummy-parameter4"].value = v;

    parameters_["dummy-parameter5"].description = "dummy boolean";
    parameters_["dummy-parameter5"].value = false;
  }

  DummySolver::~DummySolver ()
  {
  }

  void
  DummySolver::solve ()
  {
    result_ = SolverError ("The dummy solver always fail.");
  }

} // end of namespace roboptim

extern "C"
{
  using namespace roboptim;
  typedef DummySolver::parent_t solver_t;

  ROBOPTIM_CORE_DLLEXPORT std::size_t getSizeOfProblem ();
  ROBOPTIM_CORE_DLLEXPORT const char* getTypeIdOfConstraintsList ();
  ROBOPTIM_CORE_DLLEXPORT solver_t* create (const DummySolver::problem_t& pb);
  ROBOPTIM_CORE_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_CORE_DLLEXPORT std::size_t getSizeOfProblem ()
  {
    return sizeof (solver_t::problem_t);
  }

  ROBOPTIM_CORE_DLLEXPORT const char* getTypeIdOfConstraintsList ()
  {
    return typeid (solver_t::problem_t::constraintsList_t).name ();
  }

  ROBOPTIM_CORE_DLLEXPORT solver_t* create (const DummySolver::problem_t& pb)
  {
    return new DummySolver (pb);
  }

  ROBOPTIM_CORE_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
