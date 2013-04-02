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

#include <roboptim/core/plugin/nag/nag-nlp-sparse.hh>

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
    static void confun (::Integer /*ncnln*/,
			::Integer /*njnln*/,
			::Integer /*nnzjac*/,
			const double[] /*x*/,
			double[] /*conf*/,
			double[] /*conjac*/,
			Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolverNlpSparse* solver = static_cast<NagSolverNlpSparse*> (comm->p);
      assert (!!solver);
    }

    // Objective callback
    static void objfun (::Integer /*nonln*/,
			const double[] /*x*/,
			double* /*objf*/,
			double[] /*grad*/,
			Nag_Comm* comm)
    {
      assert (!!comm);
      assert (!!comm->p);
      NagSolverNlpSparse* solver = static_cast<NagSolverNlpSparse*> (comm->p);
      assert (!!solver);
    }
  } // end of namespace detail

  NagSolverNlpSparse::NagSolverNlpSparse (const problem_t& pb) throw ()
    : parent_t (pb),
      n_ (static_cast<int> (problem ().function ().inputSize ())),
      m_ (),
      ncnln_ (),
      nonln_ (),
      njnln_ (),
      iobj_ (),
      nnz_ (),
      a_ (),
      ha_ (),
      ka_ (),
      bl_ (),
      bu_ (),
      xs_ (),
      ninf_ (),
      sinf_ (),
      objf_ ()
  {
  }

  NagSolverNlpSparse::~NagSolverNlpSparse () throw ()
  {}

  void
  NagSolverNlpSparse::solve () throw ()
  {
    // Error code initialization.
    NagError fail;
    memset (&fail, 0, sizeof (NagError));
    INIT_FAIL (fail);

    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    // Options.
    Nag_E04_Opt options;
    memset (&options, 0, sizeof (Nag_E04_Opt));
    nag_opt_init (&options);

    // Solve.
    nag_opt_nlp_sparse
      (detail::confun,
       detail::objfun,
       n_, m_, ncnln_, nonln_, njnln_, iobj_, nnz_,
       &a_ (0, 0), ha_, ka_, &bl_[0], &bu_[0], &xs_[0],
       &ninf_, &sinf_, &objf_[0],
       &comm, &options, &fail);

    if (fail.code == NE_NOERROR)
      {
	Result res (problem ().function ().inputSize (),
		    problem ().function ().outputSize ());
	//FIXME:
	result_ = res;
	return;
      }

    this->result_ = SolverError (fail.message);
  }
} // end of namespace roboptim.

extern "C"
{
  typedef roboptim::NagSolverNlpSparse NagSolverNlpSparse;
  typedef roboptim::Solver<
    ::roboptim::GenericDifferentiableFunction< ::roboptim::EigenMatrixSparse>,
    boost::mpl::vector<
      ::roboptim::GenericNumericLinearFunction< ::roboptim::EigenMatrixSparse>,
      ::roboptim::GenericDifferentiableFunction< ::roboptim::EigenMatrixSparse>
      > >
  solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlpSparse::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (NagSolverNlpSparse::problem_t);
  }

  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlpSparse::problem_t& pb)
  {
    return new roboptim::NagSolverNlpSparse (pb);
  }

  ROBOPTIM_DLLEXPORT void destroy (solver_t* p)
  {
    delete p;
  }
}
