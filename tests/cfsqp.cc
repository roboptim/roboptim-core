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

using namespace optimization;
using namespace boost;
using namespace boost::lambda;

typedef CFSQPSolver solver_t;

double my_fun (const solver_t::array_t& x)
{
  return x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
}

double g_0 (const solver_t::array_t& x)
{
  return x[0] * x[1] * x[2] * x[3];
}

double g_1 (const solver_t::array_t& x)
{
  return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
}

solver_t::array_t my_gradient (const solver_t::array_t& x)
{
  solver_t::array_t grad (x.size ());

  grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
  grad[1] = x[0] * x[3];
  grad[2] = x[0] * x[3] + 1;
  grad[3] = x[0] * (x[0] + x[1] + x[2]);
  return grad;
}

solver_t::matrix_t my_hessian (const solver_t::array_t& x,
                               const solver_t::constraints_t& g,
                               double sigma_f,
                               const solver_t::array_t& lambda)
{
  solver_t::matrix_t h (x.size (), x.size ());
  h.clear ();

  h (0, 0) = sigma_f * (2 * x[3]);

  h (1, 0) = h (0, 1) = sigma_f * (x[3]);
  h (1, 1) = 0.;

  h (2, 0) = h (0, 2) = sigma_f * (x[3]);
  h (2, 1) = h (2, 2) = 0.;
  h (2, 2) = 0.;

  h (3, 0) = h (0, 3) = sigma_f * (2*x[0] + x[1] + x[2]);
  h (3, 1) = h (1, 3) = sigma_f * (x[0]);
  h (3, 2) = h (2, 3) = sigma_f * (x[0]);
  h (3, 3) = 0.;

  // Add first constraint portion.
  h (1, 0) += lambda[0] * (x[2] * x[3]);
  h (0, 1) += lambda[0] * (x[2] * x[3]);

  h (2, 0) += lambda[0] * (x[1] * x[3]);
  h (0, 2) += lambda[0] * (x[1] * x[3]);
  h (2, 1) += lambda[0] * (x[0] * x[3]);
  h (1, 2) += lambda[0] * (x[0] * x[3]);

  h (3, 0) += lambda[0] * (x[1] * x[2]);
  h (0, 3) += lambda[0] * (x[1] * x[2]);
  h (3, 1) += lambda[0] * (x[0] * x[2]);
  h (1, 3) += lambda[0] * (x[0] * x[2]);
  h (3, 2) += lambda[0] * (x[0] * x[1]);
  h (2, 3) += lambda[0] * (x[0] * x[1]);

  // Add second constraint portion.
  h (0, 0) += lambda[1] * 2;
  h (1, 1) += lambda[1] * 2;
  h (2, 2) += lambda[1] * 2;
  h (3, 3) += lambda[1] * 2;

  return h;
}

solver_t::matrix_t my_jacobian (const solver_t::array_t& x,
                                const solver_t::constraints_t& g)
{
  solver_t::matrix_t l (g.size (), x.size ());
  l.clear ();

  l(0, 0) = x[1] * x[2] * x[3];
  l(0, 1) = x[0] * x[2] * x[3];
  l(0, 2) = x[0] * x[1] * x[3];
  l(0, 3) = x[0] * x[1] * x[2];

  l(1, 0) = 2 * x[0];
  l(1, 1) = 2 * x[1];
  l(1, 2) = 2 * x[2];
  l(1, 3) = 2 * x[3];
  return l;
}

int run_test ()
{
  // Number of variables.
  static const int nvar = 4;

  // Initialize solver
  // (4 variables, using custom gradient, hessian and jacobian).
  solver_t solver (my_fun, nvar, my_gradient, my_hessian, my_jacobian);

  // Set the starting point.
  solver_t::array_t start (nvar);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  solver.setStartingPoint (start);

  // Set bound for all variables.
  // 1. < x_i < 5. (x_i in [1.;5.])
  for (int i = 0; i < nvar; ++i)
    solver.setBound (i, std::make_pair(1., 5.));

  // Define the constraints.
  solver.getConstraints ().push_back (solver_t::Constraint (g_0, 25.));
  solver.getConstraints ().push_back (solver_t::Constraint (g_1, 40., 40.));

  // Compute the minimum and retrieve the result.
  solver_t::result_t res = solver.getMinimum ();

  // Check if the minimization has succeed.
  if (res.which () != solver_t::SOLVER_VALUE)
    {
      std::cout << "A solution should have been found. Failing..."
                << std::endl;
      return 1;
    }

  // Get the result.
  solver_t::array_t& result = boost::get<solver_t::array_t> (res);

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << my_fun (result) << std::endl;
  return 0;
}


GENERATE_TEST ()
