// Copyright (C) 2013 by Benjamin Chretien, CNRS.
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

#include "shared-tests/fixture.hh"

#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver-state.hh>
#include <roboptim/core/solver.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef Solver<EigenMatrixDense> parent_solver_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

// Define a simple function.
struct F1 : public Function
{
  F1 () : Function (4, 1, "a + b + c + d")
  {}

  void impl_compute (result_ref res,
                     const_argument_ref x) const
  {
    res (0) = x[0] + x[1] + x[2] + x[3];
  }

  // No gradient, hessian.
};

// Define another simple function.
struct F2 : public Function
{
  F2 () : Function (5, 2, "a + b + c + d + e, a * b * c * d * e")
  {}

  void impl_compute (result_ref res,
                     const_argument_ref x) const
  {
    res (0) = x[0] + x[1] + x[2] + x[3] + x[4];
    res (1) = x[0] * x[1] * x[2] * x[3] * x[4];
  }

  // No gradient, hessian.
};

template <typename S>
struct DummyStruct
{
public:
  typedef S solver_t;

  void callback (const typename solver_t::problem_t&,
                 typename solver_t::solverState_t& state)
  {
    (*output) << "callback_t.callback ()" << std::endl;

    // set dummy solver state
    state.x ().fill (x_val_);
    state.cost () = cost_val_;

    (*output) << state << std::endl;
  }

  double x_val_;
  double cost_val_;
};

class DummyCallbackSolver : public parent_solver_t
{
public:
  /// Define parent's type.
  typedef parent_solver_t parent_t;
  typedef parent_t::solverState_t solverState_t;
  typedef parent_t::callback_t callback_t;

  DummyCallbackSolver (const problem_t& pb)
    : parent_t (pb),
      solverState_ (pb),
      callback_ ()
  {
  }

  ~DummyCallbackSolver ()
  {
  }

  void
  solve ()
  {
    (*output) << "solver_t.solve ()" << std::endl;

    // set dummy solver state
    solverState_.x ().setZero ();
    solverState_.cost () = 0.;
    solverState_.parameters ()["data.string"].value = std::string ("dummy data");
    solverState_.parameters ()["data.string"].description = "dummy string";
    solverState_.parameters ()["data.int"].value = int (10);
    solverState_.parameters ()["data.int"].description = "dummy integer";
    solverState_.parameters ()["data.double"].value = double (42.);
    solverState_.parameters ()["data.double"].description = "dummy double";
    solverState_.parameters ()["data.bool"].value = bool (true);
    solverState_.parameters ()["data.bool"].description = "dummy bool";
    solverState_.parameters ()["data.vector"].value = solverState_.x ();
    solverState_.parameters ()["data.vector"].description = "dummy vector";

    // call user-defined callback
    if (callback_)
      {
        // call user-defined callback
        try {
          callback_ (problem (), solverState_);
        }
        catch (boost::bad_function_call& e)
	  {
	    // Should never happen.
	    assert (0);
	  }
      }

    result_ = SolverError ("The dummy solver always fail.");
  }

  virtual void
  setIterationCallback (callback_t callback)
  {
    (*output) << "solver_t.setIterationCallback ()" << std::endl;
    callback_ = callback;
  }

  const callback_t& callback ()
  {
    return callback_;
  }

  const solverState_t& solverState ()
  {
    return solverState_;
  }

private:
  /// Current state of the solver.
  solverState_t solverState_;

  /// Intermediate callback.
  callback_t callback_;
};

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

#define ANALYZE_FUNC(N)                                         \
  /* Instantiate the functions and the problems.*/              \
  boost::shared_ptr<F##N> f##N = boost::make_shared<F##N>();    \
  solver_t::problem_t pb##N (f##N);                             \
  /* Check that the problem is well formed. */                  \
  BOOST_CHECK_EQUAL (&pb##N.function (), f##N.get());           \
  BOOST_CHECK_EQUAL (pb##N.constraints ().size (), 0u);         \
  BOOST_CHECK (!pb##N.startingPoint ());                        \
  /* Create a solver for the problem. */                        \
  boost::shared_ptr<solver_t> solver##N (new solver_t (pb##N)); \
  (*output) << *(solver##N) << std::endl << std::endl;          \
  /* Set the iteration callback. */                             \
  DummyStruct<solver_t> dummy##N;                               \
  dummy##N.x_val_ = 5;                                          \
  dummy##N.cost_val_ = 42.;                                     \
  solver##N->solve ();                                          \
  (*output) << solver##N->solverState () << std::endl;          \
  solver##N->setIterationCallback                               \
  (boost::bind (&DummyStruct<solver_t>::callback,               \
                &dummy##N, boost::placeholders::_1,             \
                boost::placeholders::_2));                      \
  solver##N->solve ();                                          \
  /* Modify callback */                                         \
  dummy##N.x_val_ = 2.;                                         \
  solver##N->solve ();                                          \
  /* Test empty callback */                                     \
  solver_t::callback_t emptyCallback##N;                        \
  solver##N->setIterationCallback (emptyCallback##N);           \
  solver##N->solve ();                                          \
  (*output) << std::endl;

BOOST_AUTO_TEST_CASE (solver_state)
{
  typedef DummyCallbackSolver solver_t;

  output = retrievePattern ("solver-state");

  // Instantiate the functions and the problems.
  ANALYZE_FUNC(1);
  (*output) << std::string (80, '*') << std::endl << std::endl;
  ANALYZE_FUNC(2);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
