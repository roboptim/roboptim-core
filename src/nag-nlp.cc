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
			const ::Integer[] /* needc */,
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
      Eigen::Map<DifferentiableFunction::result_t> ccon_ (ccon, ncnln);
      Eigen::Map<Eigen::Matrix<
	double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > jac_
	(cjac, ncnln, tdcj);

      // Iterate on constraints.
      Function::size_type idx = 0;
      typedef NagSolverNlp::problem_t::constraints_t::const_iterator iter_t;
      for (iter_t it = solver->problem ().constraints ().begin ();
	   it != solver->problem ().constraints ().end (); ++it)
	{
	  boost::shared_ptr<DifferentiableFunction> g;
	  if (it->which () == 0)
	    g = boost::get<boost::shared_ptr<NumericLinearFunction> > (*it);
	  else
	    g = boost::get<boost::shared_ptr<DifferentiableFunction> > (*it);
	  assert (!!g);

	  // evaluate constraint.
	  if (*mode == 0 || *mode == 2)
	    {
	      ccon_.segment (idx, g->outputSize ()) = (*g) (x_);
	    }

	  // evaluate jacobian.
	  if (*mode == 1 || *mode == 2)
	    {
	      jac_.block (idx, 0, g->outputSize (), g->inputSize ()) =
		g->jacobian (x_);
	    }

	  idx += g->outputSize ();
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
      assert (*mode >= 0 && *mode <= 2 && "should never happen");
      if (*mode == 0 || *mode == 2) // evaluate objective
	objf_ = solver->problem ().function () (x_);

      if (*mode == 1 || *mode == 2) // evaluate objective gradient
	grad_ = solver->problem ().function ().gradient (x_, 0);
    }
  } // end of namespace detail

  NagSolverNlp::NagSolverNlp (const problem_t& pb) throw ()
    : parent_t (pb),
      n_ (static_cast<int> (problem ().function ().inputSize ())),
      nclin_ (0),
      ncnln_ (0),
      tda_ (problem ().function ().inputSize ()),
      tdcj_ (problem ().function ().inputSize ()),
      tdh_ (problem ().function ().inputSize ()),
      objf_ (1),
      a_ (),
      bl_ (),
      bu_ (),
      ccon_ (),
      cjac_ (),
      clamda_ (),
      grad_ (),
      h_ (),
      x_ (pb.function ().inputSize ())
  {
    objf_[0] = 0.;
  }

  NagSolverNlp::~NagSolverNlp () throw ()
  {}

  void
  NagSolverNlp::solve () throw ()
  {
    // Count constraints and compute their size.
    typedef problem_t::constraints_t::const_iterator iter_t;
    for (iter_t it = problem ().constraints ().begin ();
	 it != problem ().constraints ().end (); ++it)
      {
	if (it->which () == 0)
	  {
	    boost::shared_ptr<NumericLinearFunction> g =
	      boost::get<boost::shared_ptr<NumericLinearFunction> > (*it);
	    assert (!!g);
	    nclin_ += g->outputSize ();
	  }
	else if (it->which () == 1)
	  {
	    boost::shared_ptr<DifferentiableFunction> g =
	      boost::get<boost::shared_ptr<DifferentiableFunction> > (*it);
	    assert (!!g);
	    ncnln_ += g->outputSize ();
	  }
	else
	  assert (false && "should never happen");
      }

    // Resize matrices.
    a_.resize
      (std::max (Integer (1), nclin_), problem ().function ().inputSize ());
    bl_.resize (n_ + nclin_ + ncnln_);
    bu_.resize (n_ + nclin_ + ncnln_);
    ccon_.resize (std::max (Integer (1), ncnln_));
    cjac_.resize (std::max (Integer (1), ncnln_), std::max (Integer (1), tdcj_));
    clamda_.resize (n_ + nclin_ + ncnln_);
    grad_.resize (n_);
    h_.resize (n_, n_);

    // Fill A matrix.

    Function::size_type idx = 0;
    for (iter_t it = problem ().constraints ().begin ();
	 it != problem ().constraints ().begin (); ++it)
      {
	if (it->which () != 0)
	  continue;
	boost::shared_ptr<NumericLinearFunction> g =
	  boost::get<boost::shared_ptr<NumericLinearFunction> > (*it);
	assert (!!g);

	a_.block (idx, 0, g->outputSize (), g->inputSize ()) = g->A ();
	idx += g->outputSize ();
      }

    // Fill bu and bl.

    // - x
    for (unsigned i = 0; i < problem ().argumentBounds ().size (); ++i)
      {
	bl_[i] = problem ().argumentBounds ()[i].first;
	bu_[i] = problem ().argumentBounds ()[i].second;
      }

    // - bounds for linear constraints (A)
    idx = static_cast<Function::size_type> (problem ().argumentBounds ().size ());
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () != 0)
	  continue;
	boost::shared_ptr<NumericLinearFunction> g =
	  boost::get<boost::shared_ptr<NumericLinearFunction> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	for (unsigned i = 0; i < g->outputSize (); ++i)
	  {
	    // warning: we shift bounds here.
	    bl_[idx + i] =
	      problem ().boundsVector ()[constraintId][i].first + g->b ()[i];
	    bu_[idx + i] =
	      problem ().boundsVector ()[constraintId][i].second + g->b ()[i];
	  }
	idx += g->outputSize ();
      }

    // - non-linear constraints
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () != 1)
	  continue;
	boost::shared_ptr<DifferentiableFunction> g =
	  boost::get<boost::shared_ptr<DifferentiableFunction> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	for (unsigned i = 0; i < g->outputSize (); ++i)
	  {
	    bl_[idx + i] = problem ().boundsVector ()[constraintId][i].first;
	    bu_[idx + i] = problem ().boundsVector ()[constraintId][i].second;
	  }
	idx += g->outputSize ();
      }

    // Fill starting point.
    if (problem ().startingPoint ())
      x_ = *problem ().startingPoint ();

    // Initialization
    Nag_E04State state;
    memset (&state, 0, sizeof (Nag_E04State));
    NagError fail;
    memset (&fail, 0, sizeof (NagError));
    INIT_FAIL (fail);

    // Initialize problem.
    nag_opt_nlp_init (&state, &fail);
    if (fail.code != NE_NOERROR)
      {
	this->result_ = SolverError (fail.message);
	return;
      }

    // Fill parameters.
    nag_opt_nlp_option_set_integer ("Print File", 1, &state, &fail);

    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    ::Integer majits = 0.;
    ::Integer* istate = new Integer[n_ + nclin_ + ncnln_];

    std::size_t istateSize =
      static_cast<std::size_t> ((n_ + nclin_ + ncnln_)) * sizeof (double);
    memset (istate, 0, istateSize);

    // Solve.
    nag_opt_nlp_solve
      (n_, nclin_, ncnln_, tda_, tdcj_, tdh_, &a_ (0, 0), &bl_[0], &bu_[0],
       detail::confun,
       detail::objfun,
       &majits, &istate[0], &ccon_[0], &cjac_ (0, 0), &clamda_[0], &objf_[0],
       &grad_[0], &h_(0, 0), &x_[0],
       &state, &comm, &fail);

    delete[] istate;
    istate = 0;

    if (fail.code == NE_NOERROR)
      {
	Result res (problem ().function ().inputSize (),
		    problem ().function ().outputSize ());
	res.x = x_;
	res.value = objf_;
	if (!problem ().constraints ().empty ())
	  {
	    res.constraints = ccon_;
	    res.lambda = clamda_;
	  }
	result_ = res;
	return;
      }

    this->result_ = SolverError (fail.message);
  }
} // end of namespace roboptim.

extern "C"
{
  typedef roboptim::NagSolverNlp NagSolverNlp;
  typedef roboptim::Solver<
    roboptim::DifferentiableFunction,
    boost::mpl::vector<roboptim::NumericLinearFunction,
		       roboptim::DifferentiableFunction> >
  solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ();
  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlp::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (NagSolverNlp::problem_t);
  }

  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ()
  {
    return typeid (NagSolverNlp::problem_t::constraintsList_t).name ();
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
