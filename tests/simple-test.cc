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
#include <boost/variant/get.hpp>

#include "common.hh"
#include <dummy.hh>

using namespace optimization;
using namespace boost;
using namespace boost::lambda;

typedef DummySolver solver_t;

double my_fun (const solver_t::array_t& x)
{
  return x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
}

int run_test ()
{

  // Check with identity function (fun x -> x).
  {
    solver_t solver (ret<const double&> (_1[0]), 1);
    solver_t::result_t res = solver.getMinimum ();
    boost::get<SolverError> (res);
  }

  // Check with basic function (fun x y -> x + y).
  {
    solver_t solver (ret<const double&> (_1[0]) + ret<const double&> (_1[1]), 2);
    solver_t::result_t res = solver.getMinimum ();
    boost::get<SolverError> (res);
  }

  // Check with complex function.
  {
    solver_t solver (my_fun, 4, solver_t::gradient_t ());
    solver_t::result_t res = solver.getMinimum ();
    boost::get<SolverError> (res);
  }

  return 0;
}

GENERATE_TEST ()
