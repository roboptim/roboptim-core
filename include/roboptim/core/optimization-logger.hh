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

#ifndef ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HH
# define ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HH
# include <string>

# include <boost/bind.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/filesystem.hpp>
# include <boost/filesystem/fstream.hpp>
# include <boost/format.hpp>
# include <boost/variant/apply_visitor.hpp>
# include <boost/variant/static_visitor.hpp>

# include <roboptim/core/config.hh>

namespace roboptim
{
  namespace detail
  {
    template <typename P>
    struct EvaluateConstraint : public boost::static_visitor<typename P::vector_t>
    {
      EvaluateConstraint (const typename P::vector_t& x)
	: x_ (x)
      {}

      template <typename U>
      typename P::vector_t operator () (const U& constraint) const
      {
	return constraint->operator () (x_);
      }

    private:
      typename P::vector_t x_;
    };

    template <typename P>
    struct JacobianConstraint
      : public boost::static_visitor<typename P::function_t::jacobian_t>
    {
      JacobianConstraint (const typename P::vector_t& x)
	: x_ (x)
      {}

      template <typename U>
      typename P::function_t::jacobian_t operator () (const U& constraint) const
      {
	return constraint->jacobian (x_);
      }

    private:
      typename P::vector_t x_;
    };


    struct ConstraintName : public boost::static_visitor<std::string>
    {
      template <typename U>
      std::string operator () (const U& constraint) const
      {
	return constraint->getName ();
      }
    };

    template <typename P>
    struct EvaluateConstraintViolation
    {
      typedef typename P::vector_t vector_t;
      typedef typename P::value_type value_type;
      typedef typename P::intervalsVect_t intervalsVect_t;
      typedef typename P::interval_t interval_t;

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
        else if (x_ul.second < x)
          return x - x_ul.second;
        else return 0.;
      }

      value_type uniformNorm () const
      {
        value_type norm = 0.;
        value_type cstr_viol = 0.;

        // For each multidimensional constraint
        for (size_t i = 0; i < constraints_.size (); ++i)
          {
            // For each dimension of the constraint
            for (size_t j = 0; j < constraints_[i].size (); ++j)
              {
                cstr_viol = computeViolation (constraints_[i][j],
                                              bounds_[i][j]);

                norm = std::max (norm, fabs (cstr_viol));
              }

          }
        return norm;
      }

    private:
      const std::vector<vector_t>& constraints_;
      const intervalsVect_t& bounds_;
    };
  } // end of namespace detail.

  template <typename T>
  class OptimizationLogger
  {
  public:
    typedef T solver_t;
    typedef typename solver_t::problem_t problem_t;
    typedef typename solver_t::problem_t::value_type value_type;
    typedef typename solver_t::problem_t::vector_t vector_t;
    typedef typename solver_t::problem_t::function_t::jacobian_t jacobian_t;
    typedef typename solver_t::solverState_t solverState_t;

    explicit OptimizationLogger (solver_t& solver,
				 const boost::filesystem::path& path)
      : solver_ (solver),
	path_ (path),
	output_ (),
	callbackCallId_ (0),
	firstTime_ (boost::posix_time::microsec_clock::universal_time ())
    {
      lastTime_ = firstTime_;

      // Set the callback.
      try
	{
	  solver_.setIterationCallback
            (boost::bind (&OptimizationLogger<T>::perIterationCallback,
                          this, _1, _2));
	}
      catch (std::runtime_error& e)
	{
	  std::cerr
	    << "failed to set per-iteration callback, "
	    << "many information will be missing from logs:\n"
	    << e.what () << std::endl;
	}

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

    virtual ~OptimizationLogger ()
    {
      // Unregister the callback, do not fail if this is impossible.
      try
	{
	  solver_.setIterationCallback (typename solver_t::callback_t ());
	}
      catch (std::exception& e)
	{}

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
	if (!x_.empty ())
	  {
	    for (std::size_t i = 0; i < x_[0].size (); ++i)
	      {
		if (i > 0)
		  streamX << ", ";
		streamX << "X " << i;
	      }
	    streamX << "\n";
	    for (std::size_t nIter = 0; nIter < x_.size (); ++nIter)
	      {
		for (std::size_t i = 0; i < x_[nIter].size (); ++i)
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
	    for (std::size_t i = 0; i < constraints_[0][constraintId].size (); ++i)
	      {
		if (i > 0)
		  streamConstraint << ", ";
		streamConstraint << "output " << i;
	      }
	    streamConstraint << "\n";
	    for (std::size_t nIter = 0; nIter < constraints_.size (); ++nIter)
	      {
		for (std::size_t i = 0; i < constraints_[nIter][constraintId].size (); ++i)
		  {
		    if (i > 0)
		      streamConstraint << ", ";
		    streamConstraint << constraints_[nIter][constraintId][i];
		  }
		streamConstraint << "\n";
	      }
          }
    }

  protected:
    void perIterationCallback (const problem_t& pb,
                               const solverState_t& state)
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

    virtual
    void perIterationCallbackUnsafe
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
      const typename solver_t::vector_t& x = state.x ();
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
      for (std::size_t i = 0; i < x.size (); ++i)
	{
	  streamX << x[i];
	  if (i < x.size () - 1)
	    streamX << ", ";
	}
      streamX << "\n";

      // cost
      boost::filesystem::ofstream streamCost (iterationPath / "cost");
      streamCost << cost << "\n";

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
	  nameStream << boost::apply_visitor
	    (::roboptim::detail::ConstraintName (), pb.constraints ()[constraintId])
		     << "\n";
	  // Log value
	  boost::filesystem::ofstream constraintValueStream (constraintPath / "value.csv");

	  vector_t constraintValue = boost::apply_visitor
	    (::roboptim::detail::EvaluateConstraint<problem_t> (x), pb.constraints ()[constraintId]);
	  for (std::size_t i = 0; i < constraintValue.size (); ++i)
	    {
	      constraintValueStream << constraintValue[i];
	      if (i < constraintValue.size () - 1)
		constraintValueStream << ", ";
	    }
	  constraintValueStream << "\n";
	  constraintsOneIteration[constraintId] = constraintValue;

	  // Jacobian
	  boost::filesystem::ofstream jacobianStream (constraintPath / "jacobian.csv");
	  jacobian_t jacobian = boost::apply_visitor
	    (::roboptim::detail::JacobianConstraint<problem_t> (x), pb.constraints ()[constraintId]);
	  for (std::size_t i = 0; i < jacobian.rows (); ++i)
	    {
	      for (std::size_t j = 0; j < jacobian.cols (); ++j)
		{
		  jacobianStream << jacobian.coeffRef (i, j);
		  if (j < jacobian.cols () - 1)
		    jacobianStream << ", ";
		}
	      jacobianStream << "\n";
	    }
	}
      constraints_.push_back (constraintsOneIteration);

      // constraint violation
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

      if (pb.constraints (). size () > 0)
        output_ << "- viol_g(x):" << incindent << iendl
                << cstrViol << decindent << iendl;

      output_ << std::string (80, '-') << iendl;
    }

  protected:
    const solver_t& solver () const throw ()
    {
      solver_;
    }
    solver_t& solver () throw ()
    {
      solver_;
    }

    boost::filesystem::path& path () const throw ()
    {
      return path_;
    }
    boost::filesystem::path& path () throw ()
    {
      return path_;
    }
    unsigned callbackCallId () const throw ()
    {
      return callbackCallId_;
    }

  private:
    solver_t& solver_;
    boost::filesystem::path path_;
    boost::filesystem::ofstream output_;
    unsigned callbackCallId_;
    boost::posix_time::ptime lastTime_;
    boost::posix_time::ptime firstTime_;

    std::vector<vector_t> x_;
    std::vector<value_type> costs_;
    std::vector<value_type> constraintViolations_;
    std::vector<std::vector<vector_t> > constraints_;
  };
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HH
