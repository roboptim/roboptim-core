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
#include <boost/variant/get.hpp>

#include "common.hh"
#include <dummy.hh>

using namespace optimization;

typedef DummySolver solver_t;

struct F : public Function
{
  F () : Function (4)
  {}

  virtual value_type operator () (const vector_t& x) const throw ()
  {
    return x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  // No gradient, hessian.
};

int run_test ()
{
  F f;
  Problem<Function, Function> pb (f);

  solver_t solver (pb);
  solver_t::result_t res = solver.getMinimum ();
  boost::get<SolverError> (res);

  return 0;
}

GENERATE_TEST ()
