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

#include "shared-tests/common.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/plugin/dummy.hh>

using namespace roboptim;

// Specify the solver that will be used.
typedef DummySolver solver_t;

// Define a simple function.
struct F : public Function
{
  F () : Function (4, 1, "a * d * (a + b + c) + d")
  {}

  void impl_compute (result_t& result,
		     const argument_t& argument) const throw ()
  {
    result (0) = argument[0] * argument[3]
      * (argument[0] + argument[1] + argument[2]) + argument[3];
  }

  // No gradient, hessian.
};


struct CountUnaryFunction
{
  typedef void Result;

  CountUnaryFunction (int& cnt)
    : cnt_ (cnt)
  {}

  void operator () (double x)
  {
    std::cout << "Discrete point: " << x
	      << " (cnt: " << ++cnt_ << ")"
	      << std::endl;
  }

private:
  int& cnt_;
};


int run_test ()
{
  // Instantiate the function and the problem.
  F f;
  DummySolver::problem_t pb (f);

  std::cout << pb << std::endl;

  // Check that the problem is well formed.
  assert (&pb.function () == &f);
  assert (pb.constraints ().size () == 0);
  assert (!pb.startingPoint ());

  Function::vector_t x (4);
  x.setZero ();
  pb.startingPoint () = x;
  assert (pb.startingPoint ()
	  && pb.startingPoint ()->size () == 4);

  assert (pb.boundsVector ().size () == 0);
  assert (pb.argumentBounds ().size () == 4);
  assert (pb.argumentBounds ()[0] == Function::makeInfiniteInterval ()
	  && pb.argumentBounds ()[1] == Function::makeInfiniteInterval ()
	  && pb.argumentBounds ()[2] == Function::makeInfiniteInterval ()
	  && pb.argumentBounds ()[3] == Function::makeInfiniteInterval ());


  assert (pb.scalesVector ().size () == 0);
  assert (pb.argumentScales ().size () == 4);
  assert (pb.argumentScales ()[0] == 1.
	  && pb.argumentScales ()[1] == 1.
	  && pb.argumentScales ()[2] == 1.
	  && pb.argumentScales ()[3] == 1.);

  F* g = new F ();
  pb.addConstraint (boost::shared_ptr<F> (g),
		    Function::makeInterval (0., 5.), 3.5);
  assert (pb.constraints ().size () == 1);
  assert (&pb.constraints ()[0] != 0);
  assert (pb.boundsVector ().size () == 1);
  assert (pb.boundsVector ()[0][0] == Function::makeInterval (0., 5.));
  assert (pb.scalesVector ().size () == 1);
  assert (pb.scalesVector ()[0][0] == 3.5);


  // Try to solve it with the DummySolver (direct instantiation, no plug-in).
  solver_t solver (pb);
  solver_t::result_t res = solver.minimum ();
  solver.getMinimum<SolverError> ();

  std::cout << pb << std::endl
            << "---" << std::endl
            << solver << std::endl;

  // Try to get the minimum from a GenericSolver*.
  GenericSolver* gs = &solver;
  std::cout << gs->getMinimum<SolverError> ().what ()
            << std::endl;


  // Check iteration in discrete intervals.
  {
    int cnt = 0;
    CountUnaryFunction count (cnt);

    {
      Function::discreteInterval_t interval (2.3, 3., 0.5);
      Function::foreach (interval, count);
      assert (cnt == 2);
      std::cout << std::endl;
    }

    {
      cnt = 0;
      Function::discreteInterval_t interval (2., 3., 0.1);
      Function::foreach (interval, count);
      assert (cnt == 11);
      std::cout << std::endl;
    }

    {
      cnt = 0;
      Function::discreteInterval_t interval (0.8, 10.8, 1.);
      Function::foreach (interval, count);
      assert (cnt == 11);
      std::cout << std::endl;
    }
  }

  return 0;
}

GENERATE_TEST ()
