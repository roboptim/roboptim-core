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

# include <boost/bind.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/filesystem.hpp>
# include <boost/filesystem/fstream.hpp>
# include <boost/format.hpp>
# include <boost/variant/apply_visitor.hpp>
# include <boost/variant/static_visitor.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/type_traits/is_same.hpp>

# include <roboptim/core/config.hh>

namespace roboptim
{
  namespace detail
  {
    template <typename P>
    struct EvaluateConstraintViolation
    {
      typedef typename P::vector_t        vector_t;
      typedef typename P::value_type      value_type;
      typedef typename P::size_type       size_type;
      typedef typename P::intervalsVect_t intervalsVect_t;
      typedef typename P::interval_t      interval_t;

      EvaluateConstraintViolation
      (const std::vector<vector_t>& constraints,
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
      const std::vector<vector_t>& constraints_;
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

      // If the constraint is differentiable
      template <typename U>
      void operator () (const boost::shared_ptr<U>& constraint) const
      {
        try
	  {
	    // Differentiable function: log Jacobian
	    boost::filesystem::ofstream
	      jacobianStream (constraintPath_ / "jacobian.csv");

	    // Get the Jacobian
	    jacobian_t jacobian = constraint->template castInto<differentiableFunction_t>()->jacobian (x_);

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
        catch(std::runtime_error)
	  {
	    // Do nothing
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
					     bool selfRegister)
    : solver_ (solver),
      path_ (path),
      output_ (),
      callbackCallId_ (0),
      firstTime_ (boost::posix_time::microsec_clock::universal_time ()),
      selfRegister_ (selfRegister)
  {
    lastTime_ = firstTime_;

    // Register the callback with the solver.
    if (selfRegister_) attach ();

    // Remove old logs.
    boost::filesystem::remove_all (path);
    boost::filesystem::create_directories (path);

    output_.open (path / "journal.log");

    // Display banner.
    output_
      << std::string (80, '*') << iendl
      << "RobOptim Optimization Logger" << iendl
      << " - current time: "
      << boost::posix_time::to_iso_extended_string (firstTime_) << "Z" << iendl
      << " - roboptim-core version: " ROBOPTIM_CORE_VERSION "\n"
      << std::string (80, '*') << iendl
      ;
  }

  template <typename T>
  OptimizationLogger<T>::~OptimizationLogger ()
  {
    // Unregister the callback, do not fail if this is impossible.
    if (selfRegister_) unregister ();

    // Get current time
    boost::posix_time::ptime t =
      boost::posix_time::microsec_clock::universal_time();

    output_
      << std::string (80, '*') << iendl
      << "RobOptim Optimization Logger" << iendl
      << " - current time: "
      << boost::posix_time::to_iso_extended_string(t) << "Z" << iendl
      << " - total elapsed time: "
      << (t - firstTime_) << iendl
      << std::string (80, '*') << iendl
      ;

    // Cost evolution over time.
    {
      boost::filesystem::ofstream streamCost (path_ / "cost-evolution.csv");
      streamCost << "Cost\n";
      for (std::size_t i = 0; i < costs_.size (); ++i)
	streamCost << costs_[i] << "\n";
    }

    // Constraint violation evolution over time.
    if (!constraintViolations_.empty ())
      {
	boost::filesystem::ofstream streamCstrViol
	  (path_ / "constraint-violation-evolution.csv");
	streamCstrViol << "Constraint violation\n";
	for (std::size_t i = 0; i < constraintViolations_.size (); ++i)
	  streamCstrViol << constraintViolations_[i] << "\n";
      }

    // X evolution over time.
    {
      boost::filesystem::ofstream streamX (path_ / "x-evolution.csv");
      const typename solver_t::problem_t::names_t
	argumentNames = solver_.problem ().argumentNames ();

      // Whether to print X 0, X 1 etc... or user-provided names.
      bool printDefaultX = (static_cast<size_type> (argumentNames.size ())
			    != solver_.problem ().function ().inputSize ());

      if (!x_.empty ())
	{
	  for (std::size_t i = 0;
	       i < static_cast<std::size_t> (x_[0].size ()); ++i)
	    {
	      if (i > 0)
		streamX << ", ";
	      if (printDefaultX)
		streamX << "X_" << i;
	      else streamX << argumentNames[i];
	    }
	  streamX << "\n";
	  for (std::size_t nIter = 0; nIter < x_.size (); ++nIter)
	    {
	      for (size_type i = 0; i < x_[nIter].size (); ++i)
		{
		  if (i > 0)
		    streamX << ", ";
		  streamX << x_[nIter][i];
		}
	      streamX << "\n";
	    }
	}
    }


    // Constraints evolution over time.
    if (!constraints_.empty ())
      for (std::size_t constraintId = 0; constraintId < constraints_[0].size ();
	   ++constraintId)
	{
	  boost::filesystem::ofstream streamConstraint
	    (path_ / (boost::format ("constraint-%d-evolution.csv") % constraintId).str ());
	  for (size_type i = 0; i < constraints_[0][constraintId].size (); ++i)
	    {
	      if (i > 0)
		streamConstraint << ", ";
	      streamConstraint << "output " << i;
	    }
	  streamConstraint << "\n";
	  for (std::size_t nIter = 0; nIter < constraints_.size (); ++nIter)
	    {
	      for (size_type i = 0; i < constraints_[nIter][constraintId].size (); ++i)
		{
		  if (i > 0)
		    streamConstraint << ", ";
		  streamConstraint << constraints_[nIter][constraintId][i];
		}
	      streamConstraint << "\n";
	    }
	}
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
  typename OptimizationLogger<T>::callback_t
  OptimizationLogger<T>::callback ()
  {
    return boost::bind (&OptimizationLogger<T>::perIterationCallback,
                        this, _1, _2);
  }

  template <typename T>
  template <typename U>
  typename boost::disable_if<boost::is_same<U, boost::mpl::vector<> > >::type
  OptimizationLogger<T>::process_constraints
  (const typename solver_t::problem_t& pb,
   const typename solver_t::solverState_t& state,
   const boost::filesystem::path& iterationPath,
   const_argument_ref x,
   value_type& cstrViol)
  {
    // constraints
    std::vector<vector_t> constraintsOneIteration (pb.constraints ().size ());
    for (std::size_t constraintId = 0; constraintId < pb.constraints ().size ();
	 ++constraintId)
      {
	// Create local path.
	boost::filesystem::path constraintPath =
	  iterationPath / (boost::format ("constraint-%d") % constraintId).str ();
	boost::filesystem::remove_all (constraintPath);
	boost::filesystem::create_directories (constraintPath);

	// Log name
	boost::filesystem::ofstream nameStream (constraintPath / "name");
	nameStream << pb.constraints ()[constraintId]->getName() << "\n";
	// Log value
	boost::filesystem::ofstream
	  constraintValueStream (constraintPath / "value.csv");

	vector_t constraintValue = pb.constraints ()[constraintId]->operator() (x);
	for (size_type i = 0; i < constraintValue.size (); ++i)
	  {
	    constraintValueStream << constraintValue[i];
	    if (i < constraintValue.size () - 1)
	      constraintValueStream << ", ";
	  }
	constraintValueStream << "\n";
	constraintsOneIteration[constraintId] = constraintValue;

	// Log the Jacobian (if the function is differentiable)
	::roboptim::detail::LogJacobianConstraint<problem_t> jac(x, constraintPath);
	jac(pb.constraints ()[constraintId]);
      }
    constraints_.push_back (constraintsOneIteration);

    // constraint violation: if the vector of constraints is not empty
    if (!pb.constraints (). empty ())
      {
	// if the constraint violation was not given by the solver
	if (!state.constraintViolation ())
	  {
	    // FIXME: handle argument bounds
	    ::roboptim::detail::EvaluateConstraintViolation<problem_t>
	      evalCstrViol (constraintsOneIteration, pb.boundsVector ());
	    cstrViol = evalCstrViol.uniformNorm ();
	  }
	constraintViolations_.push_back (cstrViol);

	boost::filesystem::ofstream streamCstrViol (iterationPath / "constraint-violation");
	streamCstrViol << cstrViol << "\n";

	output_ << "- viol_g(x):" << incindent << iendl
		<< cstrViol << decindent << iendl;
      }
  }


  template <typename T>
  template <typename U>
  typename boost::enable_if<boost::is_same<U, boost::mpl::vector<> > >::type
  OptimizationLogger<T>::process_constraints
  (const typename solver_t::problem_t&,
   const typename solver_t::solverState_t&,
   const boost::filesystem::path&,
   const_argument_ref,
   value_type&)
  {
    // Unconstrained problem: do nothing
  }

  template <typename T>
  void OptimizationLogger<T>::attach ()
  {
    try
      {
	solver_.setIterationCallback (callback ());
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
  (const problem_t& pb, const solverState_t& state)
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
   const typename solver_t::solverState_t& state)
  {
    // Create the iteration-specific directory.
    boost::filesystem::path iterationPath =
      path_ / (boost::format ("iteration-%d") % callbackCallId_).str ();
    boost::filesystem::remove_all (iterationPath);
    boost::filesystem::create_directories (iterationPath);

    // Compute intermediary values.
    // - Store X
    const_argument_ref x = state.x ();
    x_.push_back (x);
    // - Get current time
    boost::posix_time::ptime t =
      boost::posix_time::microsec_clock::universal_time ();
    // - Current cost
    value_type cost;
    if (!state.cost ())
      cost = pb.function ()(x)[0];
    else cost = *state.cost ();
    costs_.push_back (cost);
    // - Current constraint violation
    value_type cstrViol;
    if (!state.constraintViolation ())
      cstrViol = 0;
    else cstrViol = *state.constraintViolation ();

    // Update journal
    if (callbackCallId_ == 0)
      output_ << solver_ << iendl;

    output_
      << std::string (80, '+') << iendl
      << boost::format ("Callback call number: %d") % callbackCallId_ << iendl
      << "Elapsed time since last call: " << (t - lastTime_) << iendl
      << "- x:" << incindent << iendl
      << x << decindent << iendl
      << "- f(x):" << incindent << iendl
      << cost << decindent << iendl;

    // Log all data
    // x
    boost::filesystem::ofstream streamX (iterationPath / "x.csv");
    for (size_type i = 0; i < x.size (); ++i)
      {
	streamX << x[i];
	if (i < x.size () - 1)
	  streamX << ", ";
      }
    streamX << "\n";

    // cost
    boost::filesystem::ofstream streamCost (iterationPath / "cost");
    streamCost << cost << "\n";

    // constraints: only process if the problem is constrained
    process_constraints<typename solver_t::problem_t::constraintsList_t>
      (pb, state, iterationPath, x, cstrViol);

    output_ << std::string (80, '-') << iendl;
  }

  template <typename T>
  const boost::filesystem::path&
  OptimizationLogger<T>::logPath () const
  {
    return path_;
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

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HXX
