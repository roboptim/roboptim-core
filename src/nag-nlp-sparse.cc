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
    static void usrfun (::Integer* status,
			::Integer n,
			const double x[],
			::Integer needf,
			::Integer nf,
			double f[],
			::Integer needg,
			::Integer /*leng*/,
			double g[],
			Nag_Comm *comm)
    {
      // This is the final call, we do not have anything to do.
      if (*status >= 2)
	return;

      assert (!!comm);
      assert (!!comm->p);
      NagSolverNlpSparse* solver = static_cast<NagSolverNlpSparse*> (comm->p);
      assert (!!solver);

      Eigen::Map<const DifferentiableFunction::argument_t> x_ (x, n);

      // WARNING: the real f array is bigger than that but we map only
      // the part corresponding to the cost function.
      Eigen::Map<DifferentiableFunction::result_t> f_ (f, nf);

      typedef NagSolverNlpSparse::problem_t::constraints_t::const_iterator
	iter_t;

      // functions computation are needed
      if (needf > 0)
	{
	  f_.head (1) = solver->problem ().function () (x_);

	  unsigned offset = 1;
	  unsigned constraintId = 0;
	  for (iter_t it = solver->problem ().constraints ().begin ();
	       it != solver->problem ().constraints ().end ();
	       ++it, ++constraintId)
	    {
	      NagSolverNlpSparse::function_t::result_t res;

	      if (it->which () == 0)
		{
		  boost::shared_ptr<NagSolverNlpSparse::linearFunction_t> g =
		    boost::get<boost::shared_ptr<
		      NagSolverNlpSparse::linearFunction_t> > (*it);
		  assert (!!g);
		  f_.segment (offset, g->outputSize ()) = (*g) (x_);
		  offset += g->outputSize ();
		}
	      else if (it->which () == 1)
		{
		  boost::shared_ptr<NagSolverNlpSparse::nonlinearFunction_t> g =
		    boost::get<boost::shared_ptr<
		      NagSolverNlpSparse::nonlinearFunction_t> > (*it);
		  assert (!!g);
		  f_.segment (offset, g->outputSize ()) = (*g) (x_);
		  offset += g->outputSize ();
		}
	      else
		assert (false && "should never happen");
	    }
	}

      // gradient functions computation are needed
      if (needg > 0)
	{
	  unsigned offset = 0;
	  NagSolverNlpSparse::function_t::jacobian_t j;

	  j = solver->problem ().function ().jacobian (x_);

	  for (int k=0; k < j.outerSize (); ++k)
	    for (NagSolverNlpSparse::function_t::matrix_t::InnerIterator
		   it (j, k); it;
		 ++it, ++offset)
	      g[offset] = it.value ();

	  unsigned constraintId = 0;
	  for (iter_t it = solver->problem ().constraints ().begin ();
	       it != solver->problem ().constraints ().end ();
	       ++it, ++constraintId)
	    {
	      if (it->which () == 0)
		{
		  boost::shared_ptr<NagSolverNlpSparse::linearFunction_t> g =
		    boost::get<boost::shared_ptr<
		      NagSolverNlpSparse::linearFunction_t> > (*it);
		  assert (!!g);
		  j = g->jacobian (x_);
		}
	      else if (it->which () == 1)
		{
		  boost::shared_ptr<NagSolverNlpSparse::nonlinearFunction_t> g =
		    boost::get<boost::shared_ptr<
		      NagSolverNlpSparse::nonlinearFunction_t> > (*it);
		  assert (!!g);
		  j = g->jacobian (x_);
		}
	      else
		assert (false && "should never happen");

	      for (int k=0; k < j.outerSize (); ++k)
		for (NagSolverNlpSparse::function_t::matrix_t::InnerIterator
		       it (j, k); it; ++it)
		  g[offset] = it.value ();
	    }
	}
    }
  } // end of namespace detail

  NagSolverNlpSparse::NagSolverNlpSparse (const problem_t& pb) throw ()
    : parent_t (pb),
      nf_ (),
      n_ (pb.function ().inputSize ()),
      nxname_ (1),
      nfname_ (),
      // we do not add any offset to the objective function
      objadd_ (0.),
      // object function is always the first one
      objrow_ (1),
      prob_ (),
      iafun_ (),
      javar_ (),
      a_ (),
      lena_ (),
      nea_ (),
      igfun_ (),
      jgvar_ (),
      leng_ (),
      neg_ (),
      xlow_ (),
      xupp_ (),

      flow_ (),
      fupp_ (),

      x_ (),
      xmul_ (),
      fmul_ (),
      ns_ (0.),
      ninf_ (0.),
      sinf_ (0.)
  {
  }

  NagSolverNlpSparse::~NagSolverNlpSparse () throw ()
  {}

  void
  NagSolverNlpSparse::compute_nf ()
  {
    // Count constraints and compute their size.
    nf_ = problem ().function ().outputSize ();

    unsigned constraintId = 0;
    typedef problem_t::constraints_t::const_iterator iter_t;
    for (iter_t it = problem ().constraints ().begin ();
	 it != problem ().constraints ().end ();
	 ++it, ++constraintId)
      {
	if (it->which () == 0)
	  {
	    boost::shared_ptr<NumericLinearFunction> g =
	      boost::get<boost::shared_ptr<NumericLinearFunction> > (*it);
	    assert (!!g);
	    nf_ += g->outputSize ();
	  }
	else if (it->which () == 1)
	  {
	    boost::shared_ptr<DifferentiableFunction> g =
	      boost::get<boost::shared_ptr<DifferentiableFunction> > (*it);
	    assert (!!g);
	    nf_ += g->outputSize ();
	  }
	else
	  assert (false && "should never happen");
      }
  }

  void
  NagSolverNlpSparse::fill_xlow_xupp ()
  {
    for (unsigned i = 0; i < problem ().argumentBounds ().size (); ++i)
      {
	xlow_[i] = problem ().argumentBounds ()[i].first;
	xupp_[i] = problem ().argumentBounds ()[i].second;
      }
  }

  void
  NagSolverNlpSparse::fill_flow_fupp ()
  {
    // - bounds for cost function: always none.
    flow_[0] = function_t::infinity ();
    fupp_[0] = function_t::infinity ();

    // - bounds for linear constraints (A)
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () == 1)
	  {
	    boost::shared_ptr<linearFunction_t> g =
	      boost::get<boost::shared_ptr<linearFunction_t> >
	      (problem ().constraints ()[constraintId]);
	    assert (!!g);

	    for (function_t::size_type i = 0; i < g->outputSize (); ++i)
	      {
		// warning: we shift bounds here.
		flow_[i + 1] =
		  problem ().boundsVector ()
		  [constraintId][i].first + g->b ()[i];
		fupp_[i + 1] =
		  problem ().boundsVector ()
		  [constraintId][i].second + g->b ()[i];
	      }
	  }
	else if (problem ().constraints ()[constraintId].which () == 0)
	  {
	    boost::shared_ptr<nonlinearFunction_t> g =
	      boost::get<boost::shared_ptr<nonlinearFunction_t> >
	      (problem ().constraints ()[constraintId]);
	    assert (!!g);

	    for (function_t::size_type i = 0; i < g->outputSize (); ++i)
	      {
		xlow_[i + 1] =
		  problem ().boundsVector ()[constraintId][i].first;
		xupp_[i + 1] =
		      problem ().boundsVector ()[constraintId][i].second;
	      }

	  }
	else
	  assert (false && "should never happen");
      }
  }

  void
  NagSolverNlpSparse::fill_iafun_javar_lena_nea ()
  {
    function_t::size_type offset = 0;
    nea_ = 0;
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	// if non-linear, pass.
	if (problem ().constraints ()[constraintId].which () == 1)
	  continue;

	boost::shared_ptr<linearFunction_t> g =
	  boost::get<boost::shared_ptr<linearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	function_t::vector_t x (n_);

	// Look for a place to evaluate the jacobian of the
	// current constraint.
	// If we do not have an initial guess...
	if (!problem ().startingPoint ())
	  for (unsigned i = 0; i < x.size (); ++i)
	    {
	      // if constraint is in an interval, evaluate at middle.
	      if (problem ().boundsVector ()[constraintId][i].first
		  != function_t::infinity ()
		  &&
		  problem ().boundsVector ()[constraintId][i].second
		  != function_t::infinity ())
		x[i] =
		  (problem ().boundsVector ()
		   [constraintId][i].second
		   - problem ().boundsVector ()
		   [constraintId][i].first) / 2.;
	      // otherwise use the non-infinite bound.
	      else if (problem ().boundsVector ()
		       [constraintId][i].first
		       != function_t::infinity ())
		x[i] = problem ().boundsVector ()
		  [constraintId][i].first;
	      else
		x[i] = problem ().boundsVector ()
		  [constraintId][i].second;
	    }
	else
	  x = *(problem ().startingPoint ());

	function_t::result_t jac = (*g) (x);
	nea_ += jac.nonZeros ();

	for (int k=0; k < g->A ().outerSize (); ++k)
	  for (function_t::matrix_t::InnerIterator it (g->A (), k); it;
	       ++it, ++offset)
	    {
	      iafun_.push_back (nea_ + it.row ());
	      javar_.push_back (nea_ + it.col ());
	      a_.push_back (it.value ());
	    }
      }

    lena_ = iafun_.size ();
  }

  void
  NagSolverNlpSparse::fill_igfun_jgvar_leng_neg ()
  {
    function_t::size_type offset = 0;
    neg_ = 0;
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	// if non-linear, pass.
	if (problem ().constraints ()[constraintId].which () == 1)
	  continue;

	boost::shared_ptr<linearFunction_t> g =
	  boost::get<boost::shared_ptr<linearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	function_t::vector_t x (n_);

	// Look for a place to evaluate the jacobian of the
	// current constraint.
	// If we do not have an initial guess...
	if (!problem ().startingPoint ())
	  for (unsigned i = 0; i < x.size (); ++i)
	    {
	      // if constraint is in an interval, evaluate at middle.
	      if (problem ().boundsVector ()[constraintId][i].first
		  != function_t::infinity ()
		  &&
		  problem ().boundsVector ()[constraintId][i].second
		  != function_t::infinity ())
		x[i] =
		  (problem ().boundsVector ()
		   [constraintId][i].second
		   - problem ().boundsVector ()
		   [constraintId][i].first) / 2.;
	      // otherwise use the non-infinite bound.
	      else if (problem ().boundsVector ()
		       [constraintId][i].first
		       != function_t::infinity ())
		x[i] = problem ().boundsVector ()
		  [constraintId][i].first;
	      else
		x[i] = problem ().boundsVector ()
		  [constraintId][i].second;
	    }
	else
	  x = *(problem ().startingPoint ());

	function_t::jacobian_t jac = g->jacobian (x);
	neg_ += jac.nonZeros ();

	for (int k=0; k < jac.outerSize (); ++k)
	  for (function_t::jacobian_t::InnerIterator it (jac, k); it;
	       ++it, ++offset)
	    {
	      igfun_.push_back (neg_ + it.row ());
	      jgvar_.push_back (neg_ + it.col ());
	    }
      }

    lena_ = iafun_.size ();
  }

  void
  NagSolverNlpSparse::fill_fnames ()
  {
    fnames_.push_back (problem ().function ().getName ().c_str ());
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () == 0)
	  {
	    boost::shared_ptr<linearFunction_t> g =
	      boost::get<boost::shared_ptr<linearFunction_t> >
	      (problem ().constraints ()[constraintId]);
	    assert (!!g);

	    fnames_.push_back (g->getName ().c_str ());
	  }
	else if (problem ().constraints ()[constraintId].which () == 1)
	  {
	    boost::shared_ptr<nonlinearFunction_t> g =
	      boost::get<boost::shared_ptr<nonlinearFunction_t> >
	      (problem ().constraints ()[constraintId]);
	    assert (!!g);

	    fnames_.push_back (g->getName ().c_str ());
	  }
	else
	  assert (false && "should never happen");
      }
  }

  void
  NagSolverNlpSparse::solve () throw ()
  {
    compute_nf ();

    // we will provide function names for everyone
    nfname_ = nf_;

    // fill sparse A and G date and/or structure
    fill_iafun_javar_lena_nea ();
    fill_igfun_jgvar_leng_neg ();

    // Fill bounds.
    fill_xlow_xupp ();
    fill_flow_fupp ();

    // Fill fnames.
    fill_fnames ();

    // Fill starting point.
    if (problem ().startingPoint ())
      x_ = *problem ().startingPoint ();

    // Fill xstate.
    xstate_.resize (n_, 0.);

    // Fill f.
    fstate_.resize (nf_, 0.);

    // Fill fstate.
    fstate_.resize (nf_, 0.);

    // Error code initialization.
    NagError fail;
    memset (&fail, 0, sizeof (NagError));
    INIT_FAIL (fail);

    Nag_E04State state;
    memset (&state, 0, sizeof (Nag_E04State));

    nag_opt_sparse_nlp_init (&state, &fail);

    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    // Solve.
    const char* xnames[] = {0};

    nag_opt_sparse_nlp_solve
      (Nag_Cold,
       nf_,
       n_,
       nxname_,
       nfname_,
       objadd_,
       objrow_,
       "RobOptim problem",
       detail::usrfun,
       &iafun_[0],
       &javar_[0],
       &a_[0],
       lena_,
       nea_,
       &igfun_[0],
       &jgvar_[0],
       leng_,
       neg_,
       &xlow_[0], &xupp_[0],
       xnames,
       &flow_[0], &fupp_[0],
       &fnames_[0],
       &x_[0], &xstate_[0], &xmul_[0],
       &f_[0], &fstate_[0], &fmul_[0],
       &ns_,
       &ninf_, &sinf_,
       &state, &comm, &fail);

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
