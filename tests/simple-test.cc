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

#include "common.hh"
#include <dummy.hh>

int run_test ()
{
  using namespace optimization;
  using namespace boost::lambda;

  // Check with boost::function.

  // Check with identity function (fun x -> x).
  {
    typedef DummySolver<boost::function<double (double)> > solver_t;
    solver_t solver (_1);

    solver_t::result_t res = solver.getMinimum ();
    boost::get<SolverError> (res);
  }

  // Check with identity function (fun x y -> x + y).
  {
    typedef DummySolver<boost::function<double (double, double)> > solver_t;
    solver_t solver (_1 + _2);

    solver_t::result_t res = solver.getMinimum ();
    boost::get<SolverError> (res);
  }

  return 0;
}

GENERATE_TEST ()
