// Copyright (C) 2014 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HXX
# define ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HXX
# include <string>
# include <sstream>

# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/filesystem.hpp>
# include <boost/filesystem/fstream.hpp>
# include <boost/format.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/type_traits/is_same.hpp>

# include <roboptim/core/config.hh>
# include <roboptim/core/solver.hh>
# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  namespace detail
  {
    template <typename P>
    struct EvaluateConstraintViolation
    {
      typedef typename P::result_t        result_t;
      typedef typename P::value_type      value_type;
      typedef typename P::size_type       size_type;
      typedef typename P::intervalsVect_t intervalsVect_t;
      typedef typename P::interval_t      interval_t;

      EvaluateConstraintViolation
      (const std::vector<result_t>& constraints,
       const intervalsVect_t& bounds)
        : constraints_ (constraints),
          bounds_ (bounds)
      {}

      value_type computeViolation (const value_type& x,
                                   const interval_t& x_ul) const
      {
        if (x < x_ul.first)
          return x_ul.first - x;
        else if (x > x_ul.second)
          return x - x_ul.second;
        else return 0.;
      }

      value_type uniformNorm () const
      {
        value_type norm = 0.;
        value_type cstr_viol = 0.;

        // For each multidimensional constraint
        for (std::size_t i = 0; i < constraints_.size (); ++i)
          {
            // For each dimension of the constraint
            for (size_type j = 0; j < constraints_[i].size (); ++j)
              {
                cstr_viol = computeViolation
                  (constraints_[i][j],
                   bounds_[i][static_cast<std::size_t> (j)]);

                norm = std::max (norm, fabs (cstr_viol));
              }

          }
        return norm;
      }

    private:
      const std::vector<result_t>& constraints_;
      const intervalsVect_t& bounds_;
    };


    template <typename P>
    struct LogJacobianConstraint
    {
      /// \brief Type of the problem.
      typedef P problem_t;

      /// \brief Function traits.
      typedef typename problem_t::function_t::traits_t traits_t;

      /// \brief Type of differentiable functions.
      typedef GenericDifferentiableFunction<traits_t> differentiableFunction_t;

      /// \brief Argument type.
      typedef typename differentiableFunction_t::const_argument_ref const_argument_ref;

      /// \brief Size type.
      typedef typename problem_t::size_type size_type;

      /// \brief Jacobian type.
      typedef typename differentiableFunction_t::jacobian_t jacobian_t;

      LogJacobianConstraint
      (const_argument_ref x,
       const boost::filesystem::path& constraintPath)
        : x_ (x),
          constraintPath_ (constraintPath)
      {}

      template <typename U>
      void operator () (const boost::shared_ptr<U>& constraint) const
      {
        // If the constraint is differentiable
        if (constraint->template asType<differentiableFunction_t> ())
	  {
	    // Differentiable function: log Jacobian
	    boost::filesystem::ofstream
	      jacobianStream (constraintPath_ / "jacobian.csv");

	    // Get the Jacobian
	    jacobian_t jacobian
	      = constraint->template castInto<differentiableFunction_t> ()
	      ->jacobian (x_);

	    for (size_type i = 0;
		 i < static_cast<size_type> (jacobian.rows ()); ++i)
	      {
		for (size_type j = 0; j < jacobian.cols (); ++j)
		  {
		    jacobianStream << jacobian.coeffRef (i, j);
		    if (j < jacobian.cols () - 1)
		      jacobianStream << ", ";
		  }
		jacobianStream << "\n";
	      }
	  }
      }

    private:
      /// \brief Argument.
      const_argument_ref x_;

      /// \brief Path to the constraint log directory.
      const boost::filesystem::path& constraintPath_;
    };
  } // end of namespace detail.

  template <typename T>
  OptimizationLogger<T>::OptimizationLogger (solver_t& solver,
					     const boost::filesystem::path& path,
					     bool selfRegister,
                                             logRequest_t requests)
    : parent_t ("Optimization logger"),
      solver_ (solver),
      path_ (path),
      output_ (),
      xStream_ (),
      costStream_ (),
      constraintStreamPaths_ (),
      constraintViolationStream_ (),
      callbackCallId_ (0),
      firstTime_ (boost::posix_time::microsec_clock::universal_time ()),
      requests_ (requests),
      selfRegister_ (selfRegister)
  {
    lastTime_ = firstTime_;

    // Register the callback with the solver.
    if (selfRegister_) attach ();

    // Remove old logs.
    boost::filesystem::remove_all (path);
    boost::filesystem::create_directories (path);

    const problem_t& pb = solver_.problem ();

    // Open streams
    output_.open (path / "journal.log");

    if (isRequested (LOG_X))
      {
	xStream_.open (path / "x-evolution.csv");

	const typename solver_t::problem_t::names_t
	  argumentNames = pb.argumentNames ();

	// Whether to print X 0, X 1 etc... or user-provided names.
	size_type n = pb.function ().inputSize ();
	bool printDefaultX = (static_cast<size_type> (argumentNames.size ()) != n);

	for (std::size_t i = 0; i < static_cast<std::size_t> (n); ++i)
	  {
	    if (i > 0) xStream_ << ", ";

	    if (printDefaultX) xStream_ << "X_" << i;
	    else xStream_ << argumentNames[i];
	  }
	xStream_ << "\n";
	xStream_.flush ();
      }

    if (isRequested (LOG_CONSTRAINT_VIOLATION))
      {
	constraintViolationStream_.open (path / "constraint-violation-evolution.csv");
	constraintViolationStream_ << "Constraint violation\n";
	constraintViolationStream_.flush ();
      }

    if (isRequested (LOG_CONSTRAINT))
      {
	constraintStreamPaths_.resize (pb.constraints ().size ());
	for (std::size_t cIdx = 0; cIdx < pb.constraints ().size (); ++cIdx)
	  {
	    constraintStreamPaths_[cIdx]
              = (boost::format ("constraint-%d-evolution.csv") % cIdx).str ();
	    boost::filesystem::ofstream cStream (path_ / constraintStreamPaths_[cIdx]);

            for (size_type i = 0; i < pb.constraints ()[cIdx]->outputSize (); ++i)
	      {
		if (i > 0) cStream << ", ";
		cStream << "output " << i;
	      }

	    cStream << "\n";
	    cStream.flush ();
	  }
      }

    if (isRequested (LOG_COST))
      {
	costStream_.open (path / "cost-evolution.csv");
	costStream_ << "Cost\n";
	costStream_.flush ();
      }

    // Display banner.
    output_
      << std::string (80, '*') << iendl
      << "RobOptim Optimization Logger" << iendl
      << " - current time: "
      << boost::posix_time::to_iso_extended_string (firstTime_) << "Z" << iendl
      << " - roboptim-core version: " ROBOPTIM_CORE_VERSION "\n"
      << std::string (80, '*') << iendl;
  }

  template <typename T>
  OptimizationLogger<T>::~OptimizationLogger ()
  {
    // Unregister the callback, do not fail if this is impossible.
    if (selfRegister_) unregister ();

    output_
      << std::string (80, '*') << iendl
      << "RobOptim Optimization Logger" << iendl;

    if (isRequested (LOG_TIME))
      {
	// Get current time
	boost::posix_time::ptime t =
	  boost::posix_time::microsec_clock::universal_time();

	output_
	  << " - current time: "
	  << boost::posix_time::to_iso_extended_string(t) << "Z" << iendl
	  << " - total elapsed time: "
	  << (t - firstTime_) << iendl;
      }
    output_ << std::string (80, '*') << iendl;
  }

  template <typename T>
  void OptimizationLogger<T>::append (const std::string& text)
  {
    output_
      << std::string (80, '+') << iendl
      << text << iendl
      << std::string (80, '-') << iendl;
  }

  template <typename T>
  OptimizationLogger<T>& OptimizationLogger<T>::operator<< (const std::string& text)
  {
    append (text);
    return *this;
  }

  template <typename T>
  template <typename U>
  OptimizationLogger<T>& OptimizationLogger<T>::operator<< (const U& u)
  {
    std::stringstream ss;
    ss << u;
    append (ss.str ());
    return *this;
  }

  template <typename T>
  typename OptimizationLogger<T>::logRequest_t
  OptimizationLogger<T>::FullLogging ()
  {
    return LOG_X | LOG_COST | LOG_CONSTRAINT | LOG_CONSTRAINT_JACOBIAN |
      LOG_CONSTRAINT_VIOLATION | LOG_TIME | LOG_SOLVER;
  }

  template <typename T>
  void OptimizationLogger<T>::process_constraints
  (const typename solver_t::problem_t& pb,
   const typename solver_t::solverState_t& state,
   const boost::filesystem::path& iterationPath,
   const_argument_ref x,
   value_type& cstrViol)
  {
    // Note: only relevant if violation is not provided
    std::vector<result_t> constraintsOneIteration (pb.constraints ().size ());

    // constraints
    for (std::size_t constraintId = 0; constraintId < pb.constraints ().size ();
	 ++constraintId)
      {
	// Create local path.
	boost::filesystem::path constraintPath =
	  iterationPath / (boost::format ("constraint-%d") % constraintId).str ();
	boost::filesystem::remove_all (constraintPath);
	boost::filesystem::create_directories (constraintPath);

	// Log value
        if (isRequested (LOG_CONSTRAINT))
	  {
            // Note: need to reopen stream and append to avoid running out of
            // file handles
	    boost::filesystem::ofstream
              cStream (path_ / constraintStreamPaths_[constraintId],
                       std::ios::app);

            // TODO: avoid reallocations
	    result_t constraintValue = pb.constraints ()[constraintId]->operator() (x);
	    for (size_type i = 0; i < constraintValue.size (); ++i)
	      {
		cStream << constraintValue[i];
		if (i < constraintValue.size () - 1)
		  cStream << ", ";
	      }
	    cStream << "\n";
	    cStream.flush ();

            if (!state.constraintViolation ())
	      {
		constraintsOneIteration[constraintId] = constraintValue;
	      }
	  }

	// Log the Jacobian (if the function is differentiable)
        if (isRequested (LOG_CONSTRAINT_JACOBIAN))
	  {
	    ::roboptim::detail::LogJacobianConstraint<problem_t> jac(x, constraintPath);
	    jac(pb.constraints ()[constraintId]);
	  }
      }

    // constraint violation: if the vector of constraints is not empty
    if (!pb.constraints ().empty ())
      {
	// if the constraint violation was not given by the solver
	if (!state.constraintViolation ())
	  {
	    // FIXME: handle argument bounds
	    ::roboptim::detail::EvaluateConstraintViolation<problem_t>
	      evalCstrViol (constraintsOneIteration, pb.boundsVector ());
	    cstrViol = evalCstrViol.uniformNorm ();
	  }

        if (isRequested (LOG_CONSTRAINT_VIOLATION))
	  {
	    constraintViolationStream_ << cstrViol << "\n";
	    constraintViolationStream_.flush ();
	  }

	output_ << "- viol_g(x):" << incindent << iendl
		<< cstrViol << decindent << iendl;
      }
  }


  template <typename T>
  void OptimizationLogger<T>::attach ()
  {
    try
      {
	solver_.setIterationCallback (this->callback ());
      }
    catch (std::runtime_error& e)
      {
	std::cerr
	  << "failed to set per-iteration callback, "
	  << "many information will be missing from logs:\n"
	  << e.what () << std::endl;
      }
  }

  template <typename T>
  void OptimizationLogger<T>::unregister ()
  {
    try
      {
	solver_.setIterationCallback (typename solver_t::callback_t ());
      }
    catch (std::exception& e)
      {}
  }

  template <typename T>
  void OptimizationLogger<T>::perIterationCallback
  (const problem_t& pb, solverState_t& state)
  {
    try
      {
	perIterationCallbackUnsafe (pb, state);
      }
    catch (std::exception& e)
      {
	std::cerr << e.what () << std::endl;
      }
    catch (...)
      {
	std::cerr << "unknown exception" << std::endl;
      }

    // Turn is finished, update variables.
    lastTime_ =
      boost::posix_time::microsec_clock::universal_time ();
    ++callbackCallId_;
  }

  template <typename T>
  void OptimizationLogger<T>::perIterationCallbackUnsafe
  (const typename solver_t::problem_t& pb,
   typename solver_t::solverState_t& state)
  {
    // Create the iteration-specific directory.
    boost::filesystem::path iterationPath =
      path_ / (boost::format ("iteration-%d") % callbackCallId_).str ();
    boost::filesystem::remove_all (iterationPath);
    boost::filesystem::create_directories (iterationPath);

    // Update journal
    if (callbackCallId_ == 0)
      {
	if (isRequested (LOG_SOLVER))
	  output_ << solver_ << iendl;

	// Log the names of the constraints once
	if (isRequested (LOG_CONSTRAINT) || isRequested (LOG_CONSTRAINT_JACOBIAN))
	  {
	    boost::filesystem::ofstream nameStream (path_ / "constraint-names.csv");
	    for (std::size_t i = 0; i < pb.constraints ().size (); ++i)
	      {
		// Replace newlines from name (if any)
		std::vector<std::string> tokens
		  = split (pb.constraints ()[i]->getName (), '\n');
		std::string formatted_name = tokens[0];
		for (std::size_t j = 1; j < tokens.size (); ++j)
		  formatted_name += "\\n" + tokens[j];
		nameStream << formatted_name << "\n";
	      }
	  }
      }

    output_
      << std::string (80, '+') << iendl
      << boost::format ("Callback call number: %d") % callbackCallId_ << iendl;

    // - Get current time
    if (isRequested (LOG_TIME))
      {
	boost::posix_time::ptime t =
	  boost::posix_time::microsec_clock::universal_time ();

	output_
	  << "Elapsed time since last call: " << (t - lastTime_) << iendl;
      }

    // Store X
    const argument_t& x = state.x ();
    if (isRequested (LOG_X))
      {
	output_
	  << "- x:" << incindent << iendl
	  << x << decindent << iendl;

	for (size_type i = 0; i < x.size (); ++i)
	  {
	    xStream_ << x[i];
	    if (i < x.size () - 1)
	      xStream_ << ", ";
	  }
	xStream_ << "\n";
        xStream_.flush ();
      }

    // Current cost
    if (isRequested (LOG_COST))
      {
        value_type cost;
	if (!state.cost ())
	  cost = pb.function ()(x)[0];
	else cost = *state.cost ();
	costs_.push_back (cost);

	output_
	  << "- f(x):" << incindent << iendl
	  << cost << decindent << iendl;

	costStream_ << cost << "\n";
	costStream_.flush ();
      }

    // constraints: only process if the problem is constrained
    if ((isRequested (LOG_CONSTRAINT)
         || isRequested (LOG_CONSTRAINT_JACOBIAN))
        && !pb.constraints ().empty ())
      {
	// - Current constraint violation
	value_type cstrViol = 0.;
	if (isRequested (LOG_CONSTRAINT))
	  {
	    if (state.constraintViolation ())
	      cstrViol = *state.constraintViolation ();
	  }

	process_constraints (pb, state, iterationPath, x, cstrViol);
      }

    output_ << std::string (80, '-') << iendl;
  }

  template <typename T>
  const boost::filesystem::path&
  OptimizationLogger<T>::logPath () const
  {
    return path_;
  }

  template <typename T>
  std::ostream&
  OptimizationLogger<T>::print (std::ostream& o) const
  {
    o << this->name () << ":" << incindent;
    o << iendl << "Log directory: " << path_.string ();
    o << decindent;

    return o;
  }

  template <typename T>
  const boost::filesystem::path&
  OptimizationLogger<T>::path () const
  {
    return path_;
  }

  template <typename T>
  boost::filesystem::path&
  OptimizationLogger<T>::path ()
  {
    return path_;
  }

  template <typename T>
  const typename OptimizationLogger<T>::solver_t&
  OptimizationLogger<T>::solver () const
  {
    return solver_;
  }

  template <typename T>
  typename OptimizationLogger<T>::solver_t&
  OptimizationLogger<T>::solver ()
  {
    return solver_;
  }

  template <typename T>
  unsigned OptimizationLogger<T>::callbackCallId () const
  {
    return callbackCallId_;
  }

  template <typename T>
  bool OptimizationLogger<T>::isRequested (logRequest_t r) const
  {
    return (r & requests_) == r;
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  extern template class ROBOPTIM_CORE_DLLAPI OptimizationLogger<Solver<EigenMatrixDense> >;
  extern template class ROBOPTIM_CORE_DLLAPI OptimizationLogger<Solver<EigenMatrixSparse> >;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HXX
