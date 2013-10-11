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

#include <boost/format.hpp>

#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/numeric-linear-function.hh>

#include <nag.h>
#include <nage04.h>

#include <roboptim/core/plugin/nag/nag-nlp-sparse.hh>

#ifdef ROBOPTIM_CORE_PLUGIN_NAG_CHECK_GRADIENT
# include <roboptim/core/finite-difference-gradient.hh>
#endif // ROBOPTIM_CORE_PLUGIN_NAG_CHECK_GRADIENT


#define DEFINE_PARAMETER(KEY, DESCRIPTION, VALUE)	\
  do {							\
    this->parameters_[KEY].description = DESCRIPTION;	\
    this->parameters_[KEY].value = VALUE;		\
  } while (0)


namespace roboptim
{
  /// \internal
#ifdef ROBOPTIM_CORE_PLUGIN_NAG_CHECK_GRADIENT
  template <typename T, typename U>
  static void checkJacobian (const GenericDifferentiableFunction<T>& function,
			     int functionId,
			     U& x) throw ()
  {
    using boost::format;
    try
      {
	checkJacobianAndThrow (function, Eigen::VectorXd (x));
      }
    catch (roboptim::BadJacobian<T>& bg)
      {
	//solver.invalidateGradient ();
	std::cerr
	  << ((functionId < 0)
	      ? "Invalid cost function jacobian:"
	      : "Invalid constraint function gradient:")
	  << std::endl
	  << function.getName ()
	  << std::endl
	  << bg
	  << std::endl;
      }
  }
#else
  template <typename T, typename U>
  static void checkJacobian (const GenericDifferentiableFunction<T>&,
			     int,
			     U&) throw ()
  {}
#endif //!ROBOPTIM_CORE_CFSQP_PLUGIN_CHECK_GRADIENT


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
			::Integer leng,
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
	  // the cost function is evaluated first
	  f_.head<1> () = solver->problem ().function () (x_);

	  // then the non linear constraints
	  unsigned offset = 1;
	  unsigned constraintId = 0;

	  for (iter_t it = solver->problem ().constraints ().begin ();
	       it != solver->problem ().constraints ().end ();
	       ++it, ++constraintId)
	    {
	      NagSolverNlpSparse::function_t::result_t res;

	      if (it->which () != NagSolverNlpSparse::nonlinearFunctionId)
		continue;

	      boost::shared_ptr<NagSolverNlpSparse::nonlinearFunction_t> g =
		boost::get<boost::shared_ptr<
		  NagSolverNlpSparse::nonlinearFunction_t> > (*it);
	      assert (!!g);
	      f_.segment (offset, g->outputSize ()) = (*g) (x_);
	      offset += static_cast<unsigned> (g->outputSize ());
	    }

	  // the linear part is not taken into account but we will
	  // have to iterate through linear constraints to fetch
	  // their output size to update the offset
	  for (iter_t it = solver->problem ().constraints ().begin ();
	       it != solver->problem ().constraints ().end ();
	       ++it, ++constraintId)
	    {
	      if (it->which () != NagSolverNlpSparse::linearFunctionId)
		continue;
	      boost::shared_ptr<NagSolverNlpSparse::linearFunction_t> g =
		boost::get<boost::shared_ptr<
		  NagSolverNlpSparse::linearFunction_t> > (*it);
	      assert (!!g);
	      offset += static_cast<unsigned> (g->outputSize ());
	    }

	  assert (offset == nf);
	}

      // gradient functions computation are needed
      if (needg > 0)
	{
	  Eigen::Map<DifferentiableFunction::vector_t> g_ (g, leng);

	  unsigned offset = 0;
	  NagSolverNlpSparse::function_t::jacobian_t j;

	  // retrieve objective jacobian
	  j = solver->problem ().function ().jacobian (x_);

	  checkJacobian (solver->problem ().function (), -1, x_);

	  for (int k = 0; k < j.outerSize (); ++k)
	    for (NagSolverNlpSparse::function_t::matrix_t::InnerIterator
		   it (j, k); it; ++it)
	      g_[offset++] = it.value ();

	  NagSolverNlpSparse::function_t::size_type constraintId = 0;
	  for (iter_t it = solver->problem ().constraints ().begin ();
	       it != solver->problem ().constraints ().end ();
	       ++it, ++constraintId)
	    {
	      if (it->which () != NagSolverNlpSparse::nonlinearFunctionId)
		continue;

	      boost::shared_ptr<NagSolverNlpSparse::nonlinearFunction_t> g =
		boost::get<boost::shared_ptr<
		  NagSolverNlpSparse::nonlinearFunction_t> > (*it);
	      assert (!!g);
	      j = g->jacobian (x_);
	      checkJacobian (*g, constraintId, x_);

	      for (int k = 0; k < j.outerSize (); ++k)
		for (NagSolverNlpSparse::function_t::matrix_t::InnerIterator
		       it (j, k); it; ++it)
		  g_[offset++] = it.value ();
	    }
	  assert (offset == leng);
	}

      if (!solver->callback ())
	return;
      DifferentiableFunction::vector_t xCb = x_;
      solver->callback () (xCb, solver->problem ());
    }
  } // end of namespace detail

  NagSolverNlpSparse::NagSolverNlpSparse (const problem_t& pb) throw ()
    : parent_t (pb),
      nf_ (),
      n_ (pb.function ().inputSize ()),
      nxname_ (),
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
      xnames_ (),

      flow_ (),
      fupp_ (),
      fnames_ (),

      x_ (pb.function ().inputSize ()),
      xmul_ (pb.function ().inputSize ()),
      fmul_ (pb.function ().inputSize ()),
      ns_ (0.),
      ninf_ (0.),
      sinf_ (0.)
  {
    // Shared parameters.
    DEFINE_PARAMETER ("max-iterations", "number of iterations", 3000);

    // NAG specific.

    //  Output
    DEFINE_PARAMETER ("nag.print-file", "log file", 1);
    DEFINE_PARAMETER ("nag.verify-level", "verify level", 3);
  }

  NagSolverNlpSparse::~NagSolverNlpSparse () throw ()
  {
    // functions and variables names are allocated by strdup so we
    // need to call free, unfortunately Nag API requires a C-array of
    // const char* so we const_cast here to be able to free the
    // memory.
    for (std::size_t i = 0; i < xnames_.size (); ++i)
      free (const_cast<char*> (xnames_[i]));

    for (std::size_t i = 0; i < fnames_.size (); ++i)
      free (const_cast<char*> (fnames_[i]));
  }

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
	if (it->which () == linearFunctionId)
	  {
	    boost::shared_ptr<linearFunction_t> g =
	      boost::get<boost::shared_ptr<linearFunction_t> > (*it);
	    assert (!!g);
	    nf_ += g->outputSize ();
	  }
	else if (it->which () == nonlinearFunctionId)
	  {
	    boost::shared_ptr<nonlinearFunction_t> g =
	      boost::get<boost::shared_ptr<nonlinearFunction_t> > (*it);
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
    assert (problem ().argumentBounds ().size () ==
	    static_cast<std::size_t> (n_));

    xlow_.resize (n_);
    xupp_.resize (n_);

    for (unsigned i = 0; i < n_; ++i)
      {
	xlow_[i] = problem ().argumentBounds ()[i].first;
	xupp_[i] = problem ().argumentBounds ()[i].second;
      }
  }

  void
  NagSolverNlpSparse::fill_flow_fupp ()
  {
    flow_.resize (nf_);
    fupp_.resize (nf_);
    flow_.setZero ();
    fupp_.setZero ();

    // - bounds for cost function: always none.
    flow_[0] = -function_t::infinity ();
    fupp_[0] = function_t::infinity ();

    // - bounds for non-linear constraints
    int offset = 1; // start at one because of cost function.
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which ()
	    != nonlinearFunctionId)
	  continue;

	boost::shared_ptr<nonlinearFunction_t> g =
	  boost::get<boost::shared_ptr<nonlinearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	for (Function::size_type i = 0; i < g->outputSize (); ++i)
	  {
	    std::size_t i_ = static_cast<std::size_t> (i);
	    flow_[offset] =
	      problem ().boundsVector ()[constraintId][i_].first;
	    fupp_[offset] =
	      problem ().boundsVector ()[constraintId][i_].second;
	    ++offset;
	  }
      }

    // - bounds for linear constraints
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which ()
	    != linearFunctionId)
	  continue;

	boost::shared_ptr<linearFunction_t> g =
	  boost::get<boost::shared_ptr<linearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	numericLinearFunction_t g_ (*g);

	for (function_t::size_type i = 0; i < g->outputSize (); ++i)
	  {
	    std::size_t i_ = static_cast<std::size_t> (i);
	    // warning: we shift bounds here.
	    flow_[offset] =
	      problem ().boundsVector ()
	      [constraintId][i_].first - g_.b ()[i];
	    fupp_[offset] =
	      problem ().boundsVector ()
	      [constraintId][i_].second - g_.b ()[i];
	    ++offset;
	  }
      }

    // Make sure we fill the vector entirely.
    assert (offset == nf_);

    // Make sure the bounds are consistent.
    for (int id = 0; id < nf_; ++id)
      {
	if (std::abs (flow_[id]) < 1e-6)
	  flow_[id] = 0.;
	if (std::abs (fupp_[id]) < 1e-6)
	  fupp_[id] = 0.;
	if (std::abs (flow_[id] - fupp_[id]) < 1e-6)
	  flow_[id] = fupp_[id];
	assert (flow_[id] <= fupp_[id]);
      }

  }

  NagSolverNlpSparse::function_t::vector_t
  NagSolverNlpSparse::lookForX ()
  {
    function_t::vector_t x (n_);

    if (!problem ().startingPoint ())
      x.setZero ();
    else
      x = *(problem ().startingPoint ());
    return x;
  }

  NagSolverNlpSparse::function_t::vector_t
  NagSolverNlpSparse::lookForX (unsigned constraintId)
  {
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
    return x;
  }

  void
  NagSolverNlpSparse::fill_iafun_javar_lena_nea ()
  {
    iafun_.clear ();
    javar_.clear ();
    a_.clear ();

    function_t::size_type offset = 0;

    // compute the intial offset
    offset += problem ().function ().outputSize ();
    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	// if linear, pass.
	if (problem ().constraints ()[constraintId].which ()
	    != nonlinearFunctionId)
	  continue;

	boost::shared_ptr<NagSolverNlpSparse::nonlinearFunction_t> g =
	  boost::get<boost::shared_ptr<
	    NagSolverNlpSparse::nonlinearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);
	offset += static_cast<unsigned> (g->outputSize ());
      }

    nea_ = 0;

    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	// if non-linear, pass.
	if (problem ().constraints ()[constraintId].which ()
	    != linearFunctionId)
	  continue;

	boost::shared_ptr<linearFunction_t> g =
	  boost::get<boost::shared_ptr<linearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);
	numericLinearFunction_t g_ (*g);

	// copy the non-null elements of the jacobian
	for (int k=0; k < g_.A ().outerSize (); ++k)
	  for (function_t::matrix_t::InnerIterator it (g_.A (), k); it; ++it)
	    {
	      iafun_.push_back (offset + it.row () + 1);
	      javar_.push_back (it.col () + 1);
	      a_.push_back (it.value ());
	    }
	offset += static_cast<int> (g_.A ().rows ());
      }

    lena_ = static_cast<int> (iafun_.size ());
    nea_ = lena_;

    if (lena_ == 0)
      {
	iafun_.resize (1);
	javar_.resize (1);
	a_.resize (1);
	lena_ = 1;
	nea_ = 0;
      }
  }

  void
  NagSolverNlpSparse::fill_igfun_jgvar_leng_neg ()
  {
    igfun_.clear ();
    jgvar_.clear ();

    function_t::size_type offset = 0;
    neg_ = 0;

    // evaluate objective jacobian.
    function_t::vector_t x = lookForX ();
    function_t::jacobian_t jac =
      problem ().function ().jacobian (x);
    neg_ += jac.nonZeros ();

    for (int k=0; k < jac.outerSize (); ++k)
      for (function_t::jacobian_t::InnerIterator it (jac, k); it; ++it)
	{
	  igfun_.push_back (it.row () + 1);
	  jgvar_.push_back (it.col () + 1);
	}
    offset += jac.rows ();

    for (unsigned constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	// if linear, pass.
	if (problem ().constraints ()[constraintId].which () !=
	    nonlinearFunctionId)
	  continue;

	boost::shared_ptr<nonlinearFunction_t> g =
	  boost::get<boost::shared_ptr<nonlinearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	function_t::vector_t x = lookForX (constraintId);
	function_t::jacobian_t jac = g->jacobian (x);
	neg_ += jac.nonZeros ();

	for (int k=0; k < jac.outerSize (); ++k)
	  for (function_t::jacobian_t::InnerIterator it (jac, k); it;
	       ++it)
	    {
	      igfun_.push_back (offset + it.row () + 1);
	      jgvar_.push_back (it.col () + 1);
	    }
	offset += jac.rows ();
      }

    leng_ = static_cast<int> (igfun_.size ());

    if (leng_ == 0)
      {
	igfun_.resize (1);
	jgvar_.resize (1);
	leng_ = 1;
	neg_ = 0;
      }
  }

  void
  NagSolverNlpSparse::fill_fnames ()
  {
    boost::format fmt
      ("%1%, %2%, Ouput variable %3%");


    // first push the cost function name
    fnames_.push_back
      (strdup
       ((fmt
	 % "cost"
	 % problem ().function ().getName ()
	 % 0).str ().c_str ()));

    // then non-linear constraints
    for (std::size_t constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () !=
	    nonlinearFunctionId)
	  continue;

	boost::shared_ptr<nonlinearFunction_t> g =
	  boost::get<boost::shared_ptr<nonlinearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	for (Function::size_type i = 0; i < g->outputSize (); ++i)
	  fnames_.push_back
	    (strdup
	     ((fmt
	       % "non-linear"
	       % g->getName ()
	       % i).str ().c_str ()));
      }

    // and to finish the linear ones.
    for (std::size_t constraintId = 0;
	 constraintId < problem ().constraints ().size ();
	 ++constraintId)
      {
	if (problem ().constraints ()[constraintId].which () !=
	    linearFunctionId)
	  continue;

	boost::shared_ptr<linearFunction_t> g =
	  boost::get<boost::shared_ptr<linearFunction_t> >
	  (problem ().constraints ()[constraintId]);
	assert (!!g);

	for (Function::size_type i = 0; i < g->outputSize (); ++i)
	  fnames_.push_back
	    (strdup
	     ((fmt
	       % "linear"
	       % g->getName ()
	       % i).str ().c_str ()));
      }

    assert (nf_ == static_cast<int> (fnames_.size ()));
  }

  const char* cxxtoCString (std::string s)
  {
    return s.c_str ();
  }

  void
  NagSolverNlpSparse::solve () throw ()
  {
    compute_nf ();

    if (nf_ == 1 || n_ == 1)
      {
	nfname_ = 1.;
	nxname_ = 1.;
      }
    else
      {
	nfname_ = nf_;
	nxname_ = n_;
      }

    // fill sparse A and G date and/or structure
    fill_iafun_javar_lena_nea ();
    fill_igfun_jgvar_leng_neg ();

    // Fill bounds.
    fill_xlow_xupp ();
    fill_flow_fupp ();

    // Fill fnames.
    fill_fnames ();

    // Fill xstate.
    x_.resize (n_);
    xstate_.resize (static_cast<std::size_t> (n_));
    xmul_.resize (static_cast<Eigen::MatrixXd::Index> (n_));

    // Fill starting point.
    if (problem ().startingPoint ())
      x_ = *problem ().startingPoint ();
    else
      x_.setZero ();

    // Fill f, fstate, fmul.
    f_.resize (nf_);
    fstate_.resize (static_cast<std::size_t> (nf_));
    fmul_.resize (nf_);

    // Error code initialization.
    NagError fail;
    memset (&fail, 0, sizeof (NagError));
    INIT_FAIL (fail);

    Nag_E04State state;
    memset (&state, 0, sizeof (Nag_E04State));

    nag_opt_sparse_nlp_init (&state, &fail);

    // Set parameters.
    nag_opt_sparse_nlp_option_set_integer
      ("Major Iterations Limit",
       this->getParameter<int> ("max-iterations"), &state, &fail);

    nag_opt_sparse_nlp_option_set_integer
      ("Print file",
       this->getParameter<int> ("nag.print-file"), &state, &fail);
    nag_opt_sparse_nlp_option_set_integer
      ("Verify Level",
       this->getParameter<int> ("nag.verify-level"), &state, &fail);


    // Nag communication object.
    Nag_Comm comm;
    memset (&comm, 0, sizeof (Nag_Comm));
    comm.p = this;

    // Solve.
    for (Function::size_type i = 0; i < problem_.function ().inputSize (); ++i)
      xnames_.push_back
	(strdup
	 (
	  ((boost::format ("RobOptim variable %1%") % i).str ().c_str ())
	  ));

    // Double check that sizes are valid.
    assert (nf_ > 0);
    assert (n_ > 0);
    assert (nxname_ == 1 || nxname_ == n_);
    assert (nfname_ == 1 || nfname_ == nf_);
    assert (objadd_ == 0.);
    assert (1 <= objrow_ && objrow_ <= nf_);
    assert (iafun_.size () == static_cast<std::size_t> (lena_));
    assert (javar_.size () == static_cast<std::size_t> (lena_));
    assert (a_.size () == static_cast<std::size_t> (lena_));
    assert (lena_ >= 1);

    assert (igfun_.size () >= static_cast<std::size_t> (leng_));
    assert (jgvar_.size () >= static_cast<std::size_t> (leng_));
    assert (leng_ >= 1);
    assert (0 <= neg_ && neg_ <= leng_);

    assert (xlow_.size () == static_cast<Eigen::MatrixXd::Index> (n_));
    assert (xupp_.size () == static_cast<Eigen::MatrixXd::Index> (n_));

    assert (flow_.size () == static_cast<Eigen::MatrixXd::Index> (nf_));
    assert (fupp_.size () == static_cast<Eigen::MatrixXd::Index> (nf_));

    assert (x_.size () == static_cast<Eigen::MatrixXd::Index> (n_));
    assert (xstate_.size () == static_cast<std::size_t> (n_));
    assert (xmul_.size () == static_cast<Eigen::MatrixXd::Index> (n_));

    assert (f_.size () == static_cast<Eigen::MatrixXd::Index> (nf_));
    assert (fstate_.size () == static_cast<std::size_t> (nf_));
    assert (fmul_.size () == static_cast<Eigen::MatrixXd::Index> (nf_));

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
       &xnames_[0],
       &flow_[0], &fupp_[0],
       &fnames_[0],
       &x_[0], &xstate_[0], &xmul_[0],
       &f_[0], &fstate_[0], &fmul_[0],
       &ns_,
       &ninf_, &sinf_,
       &state, &comm, &fail);

    Result res (problem ().function ().inputSize (),
		problem ().function ().outputSize ());

    res.x = x_;
    res.value.setZero ();
    res.value[0] = f_[0];
    res.constraints = f_.segment (1, nf_ - 1);
    res.lambda = fmul_.segment (1, nf_ - 1);


    if (fail.code == NE_NOERROR)
      {
	this->result_ = res;
	return;
      }

    SolverError error (fail.message);
    error.lastState () = res;
    this->result_ = error;
  }
} // end of namespace roboptim.

extern "C"
{
  typedef roboptim::NagSolverNlpSparse NagSolverNlpSparse;
  typedef roboptim::Solver<
    ::roboptim::GenericDifferentiableFunction< ::roboptim::EigenMatrixSparse>,
    boost::mpl::vector<
      ::roboptim::GenericLinearFunction< ::roboptim::EigenMatrixSparse>,
      ::roboptim::GenericDifferentiableFunction< ::roboptim::EigenMatrixSparse>
      > >
  solver_t;

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ();
  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ();
  ROBOPTIM_DLLEXPORT solver_t* create
  (const NagSolverNlpSparse::problem_t& pb);
  ROBOPTIM_DLLEXPORT void destroy (solver_t* p);

  ROBOPTIM_DLLEXPORT unsigned getSizeOfProblem ()
  {
    return sizeof (NagSolverNlpSparse::problem_t);
  }

  ROBOPTIM_DLLEXPORT const char* getTypeIdOfConstraintsList ()
  {
    return typeid (NagSolverNlpSparse::problem_t::constraintsList_t).name ();
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
