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

#include <cfsqp.hh>

#include "common.hh"
#include "problem.hh"
#include "hs071.hh"

int run_test ()
{
  Problem pb = Problem (F ());

  // Set the starting point.
  Function::vector_t start (pb.function.n);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  pb.start = start;

  pb.constraints.push_back (Problem::functionPtr_t (new G0 ()));
  pb.constraints.push_back (Problem::functionPtr_t (new G1 ()));


  // Initialize solver
  CFSQPSolver solver (pb);

  // Compute the minimum and retrieve the result.
  CFSQPSolver::result_t res = solver.getMinimum ();

  // Check if the minimization has succeed.
  if (res.which () != CFSQPSolver::SOLVER_VALUE)
    {
      std::cout << "A solution should have been found. Failing..."
                << std::endl;
      return 1;
    }

  // Get the result.
  CFSQPSolver::vector_t& result = boost::get<CFSQPSolver::vector_t> (res);

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << pb.function (result) << std::endl;
  return 0;
}


GENERATE_TEST ()
