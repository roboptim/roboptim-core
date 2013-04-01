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

#include <roboptim/core/differentiable-function.hh>

#include <nag.h>
#include <nage04.h>

#include <roboptim/core/plugin/nag/nag-nlp.hh>

#define DEFINE_PARAMETER(KEY, DESCRIPTION, VALUE)	\
  do {							\
    this->parameters_[KEY].description = DESCRIPTION;	\
    this->parameters_[KEY].value = VALUE;		\
  } while (0)


namespace roboptim
{
  namespace detail
  {
    // Constraints Callback
    static void confun (::Integer* mode,
			::Integer ncnln,
			::Integer n,
			::Integer tdcj,
			const ::Integer needc[],
			const double x[],
			double ccon[],
			double cjac[],
			::Integer /*nstate*/,
			Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolverNlp* solver = static_cast<NagSolverNlp*> (comm->p);
      assert (!!solver);

      // Maps C-arrays to Eigen structures.
      Eigen::Map<const DifferentiableFunction::argument_t> x_ (x, n);

      if (*mode == 0)
	{
	}
      if (*mode == 1)
	{
	}
      if (*mode == 2)
	{
	}
    }

    // Objective callback
    static void objfun (::Integer* mode,
			::Integer n,
			const double x[],
			double* objf,
			double grad[],
			::Integer /*nstate*/,
			Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolverNlp* solver = static_cast<NagSolverNlp*> (comm->p);
      assert (!!solver);

      // Maps C-arrays to Eigen structures.
      Eigen::Map<const DifferentiableFunction::argument_t> x_ (x, n);
      Eigen::Map<DifferentiableFunction::result_t> objf_ (objf, 1);
      Eigen::Map<DifferentiableFunction::gradient_t> grad_ (grad, n);


      assert (!!mode);
      assert ((*mode >= 0 || *mode <= 2) && "should never happen");
      if (*mode == 0 || *mode == 3) // evaluate objective
	objf_ = solver->problem ().function () (x_);

      if (*mode == 1 || *mode == 3) // evaluate objective gradient
	grad_ = solver->problem ().function ().gradient () (x_, 0);
    }
  } // end of namespace detail

  NagSolverNlp::NagSolverNlp (const problem_t& pb) throw ()
    : parent_t (pb),
      x_ (),
      f_ (problem ().function ().outputSize ()),
      g_ (problem ().function ().inputSize ())
  {
  }

  NagSolverNlp::~NagSolverNlp () throw ()
  {}

  void
  NagSolverNlp::solve () throw ()
  {
    // Initialization
    Nag_E04State state;
    NagError fail;
    nag_opt_nlp_init (&state, &fail);

    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    ::Integer n = problem ().function ().inputSize ();
    ::Integer nclin;
    ::Integer ncnln;
    ::Integer tda;
    ::Integer tdcj;
    ::Integer tdh;
    double a[1];
    double bl[1];
    double bu[1];
    ::Integer majits;
    ::Integer istate[1];
    double ccon[1];
    double cjac[1];
    double clamda[1];
    double objf;
    double grad[1];
    double h[1];
    double x[1];

    nag_opt_nlp_solve
      (n, nclin, ncnln, tda, tdcj, tdh, a, bl, bu,
       detail::confun,
       detail::objfun,
       &majits, istate, ccon, cjac, clamda, &objf, grad, h, x, 
       &state, &comm, &fail);
  }
} // end of namespace roboptim.

extern "C"
{
  typedef roboptim::NagSolverNlp NagSolverNlp;
  typedef roboptim::Solver<
    roboptim::DifferentiableFunction,
    boost::mpl::vector<roboptim::LinearFunction,
		       roboptim::DifferentiableFunction> >
  solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlp::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (NagSolverNlp::problem_t);
  }

  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlp::problem_t& pb)
  {
    return new roboptim::NagSolverNlp (pb);
  }

  ROBOPTIM_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
