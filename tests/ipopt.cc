// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/variant/get.hpp>

#include <coin/IpIpoptApplication.hpp>

#include <liboptimization/ipopt.hh>

#include "common.hh"
#include "hs071.hh"

int run_test ()
{
  F f;
  G0 g0;
  G1 g1;

  Problem<TwiceDerivableFunction, TwiceDerivableFunction> pb (f);
  pb.getConstraints ().push_back (&g0);
  pb.getConstraints ().push_back (&g1);

  // Set the starting point.
  Function::vector_t start (f.n);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  pb.getStartingPoint () = start;

  // Initialize solver
  IpoptSolver solver (pb);

  // Compute the minimum and retrieve the result.
  IpoptSolver::result_t res = solver.getMinimum ();

  // Check if the minimization has succeed.
  if (res.which () != IpoptSolver::SOLVER_VALUE)
    {
      std::cout << "A solution should have been found. Failing..."
                << std::endl;
      return 1;
    }

  // Get the result.
  IpoptSolver::vector_t& result = boost::get<IpoptSolver::vector_t> (res);

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << solver.getProblem ().getFunction () (result) << std::endl;
  return 0;
}


GENERATE_TEST ()
