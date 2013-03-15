// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS.
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

#define DEFINE_PARAMETER(KEY, DESCRIPTION, VALUE)	\
  do {							\
    this->parameters_[KEY].description = DESCRIPTION;	\
    this->parameters_[KEY].value = VALUE;		\
  } while (0)


namespace roboptim
{
  namespace detail
  {
    static void
    nagSolverCallback (double xc, double *fc, Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolver* solver = static_cast<NagSolver*> (comm->p);
      assert (!!solver);

      Eigen::Map<const Function::vector_t> x_
	(&xc, 1);
      Eigen::Map<Function::vector_t> fc_
	(fc, solver->problem ().function ().outputSize ());

      fc_.setZero ();
      fc_ = solver->problem ().function () (x_);
    }
  } // end of namespace detail

  NagSolver::NagSolver (const problem_t& pb) throw ()
    : parent_t (pb),
      e1_ (0.),
      e2_ (0.),
      a_ (problem ().function ().inputSize ()),
      b_ (problem ().function ().inputSize ()),
      x_ (1),
      f_ (problem ().function ().outputSize ())
  {
    if (pb.function ().inputSize () != 1)
      throw std::runtime_error
	("this solver only support cost function which input size is 1");

    // Argument lower (a) and upper (b) bounds.
    assert (static_cast<Function::size_type>
	    (problem ().argumentBounds ().size ()) ==
	    problem ().function ().inputSize ());

    for (unsigned i = 0; i < problem ().argumentBounds ().size (); ++i)
      a_[i] = problem ().argumentBounds ()[i].first,
	b_[i] = problem ().argumentBounds ()[i].second;

    x_.setZero ();
    f_.setZero ();

    // Shared parameters.
    DEFINE_PARAMETER ("max-iterations", "number of iterations", 30);

    // Custom parameters
    DEFINE_PARAMETER ("nag.e1", "relative accuracy (0 means default)", 0.);
    DEFINE_PARAMETER ("nag.e2", "absolute accuracy (0 means default)", 0.);
  }

  NagSolver::~NagSolver () throw ()
  {}

  void
  NagSolver::solve () throw ()
  {
    // e1 and e2
    e1_ = boost::get<double> (this->parameters_["nag.e1"].value);
    e2_ = boost::get<double> (this->parameters_["nag.e2"].value);

    // Number of iterations
    Integer max_fun =
      boost::get<int> (this->parameters_["max-iterations"].value);

    // Solution.
    if (problem ().startingPoint ())
      x_ = *(problem ().startingPoint ());

    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    // Nag error code.
    NagError fail;
    memset (&fail, 0, sizeof (NagError));
    INIT_FAIL (fail);

    nag_opt_one_var_no_deriv
      (detail::nagSolverCallback,
       e1_, e2_, &a_[0], &b_[0], max_fun, &x_[0], &f_[0], &comm, &fail);

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

extern "C"
{
  typedef roboptim::NagSolver NagSolver;
  typedef roboptim::Solver<roboptim::Function,
			   boost::mpl::vector<> > solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT solver_t* create (const NagSolver::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (NagSolver::problem_t);
  }

  ROBOPTIM_DLLEXPORT solver_t* create (const NagSolver::problem_t& pb)
  {
    return new roboptim::NagSolver (pb);
  }

  ROBOPTIM_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
