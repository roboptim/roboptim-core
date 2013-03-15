// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#include <cassert>
#include <cstring>

#include <roboptim/core/function.hh>

#include <nag.h>
#include <nage04.h>

#include <roboptim/core/plugin/nag/nag.hh>

namespace roboptim
{
  namespace detail
  {
    void
    nagSolverCallback (double xc, double *fc, Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolver* solver = static_cast<NagSolver*> (comm->p);
      assert (!!solver);

      Eigen::Map<const Function::vector_t> x_
	(&xc, solver->problem ().function ().inputSize ());
      Eigen::Map<Function::vector_t> fc_
	(fc, solver->problem ().function ().outputSize ());

      fc_ = solver->problem ().function () (x_);
    }
  } // end of namespace detail

  NagSolver::NagSolver (const problem_t& pb) throw ()
    : parent_t (pb)
  {}

  NagSolver::~NagSolver () throw ()
  {}

  void
  NagSolver::solve () throw ()
  {
    // Result relative accuracy.
    double e1 = 0.;
    // Result absolute accuracy.
    double e2 = 0.;

    // Argument lower (a) and upper (b) bounds.
    assert (static_cast<Function::size_type>
	    (problem ().argumentBounds ().size ()) ==
	    problem ().function ().inputSize ());

    double* a = new double [problem ().function ().inputSize ()];
    double* b = new double [problem ().function ().inputSize ()];

    for (unsigned i = 0; i < problem ().argumentBounds ().size (); ++i)
      a[i] = problem ().argumentBounds ()[i].first,
	b[i] = problem ().argumentBounds ()[i].second;

    // Number of iterations
    Integer max_fun = 30;

    // Solution.
    double* x = new double [problem ().function ().inputSize ()];
    Eigen::Map<Function::vector_t> x_
      (x, problem ().function ().inputSize ());

    memset (x, 0, problem ().function ().inputSize () * sizeof (double));
    if (problem ().startingPoint ())
      x_ = *(problem ().startingPoint ());

    // Final cost.
    double* f = new double [problem ().function ().outputSize ()];
    Eigen::Map<Function::vector_t> f_
      (f, problem ().function ().outputSize ());
    memset (f, 0, problem ().function ().outputSize () * sizeof (double));

    // Nag communication object.
    Nag_Comm comm;
    comm.user = 0, comm.iuser = 0, comm.p = this;

    // Nag error code.
    NagError fail;
    INIT_FAIL (fail);

    nag_opt_one_var_no_deriv
      (detail::nagSolverCallback,
       e1, e2, a, b, max_fun, x, f, &comm, &fail);

    if (fail.code == NE_NOERROR)
      {
	Result res (problem ().function ().inputSize (),
		    problem ().function ().outputSize ());
	res.x = x_;
	res.value = f_;
	result_ = res;
	return;
      }

    this->result_ = SolverError (fail.message);
  }
} // end of namespace roboptim.
