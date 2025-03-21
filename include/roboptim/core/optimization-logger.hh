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

# include <boost/bind/bind.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/filesystem.hpp>
# include <boost/filesystem/fstream.hpp>
# include <boost/format.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/type_traits/is_same.hpp>

# include <roboptim/core/config.hh>
# include <roboptim/core/portability.hh>
# include <roboptim/core/solver-callback.hh>

namespace roboptim
{
  /// \brief Log the optimization process (values, Jacobians, time taken
  /// etc.).
  /// \tparam S solver type.
  template <typename S>
  class OptimizationLogger : public SolverCallback<S>
  {
  public:
    typedef SolverCallback<S> parent_t;

    typedef S solver_t;
    typedef typename solver_t::problem_t                       problem_t;
    typedef typename solver_t::problem_t::value_type           value_type;
    typedef typename solver_t::problem_t::size_type            size_type;
    typedef typename solver_t::problem_t::result_t             result_t;
    typedef typename solver_t::solverState_t                   solverState_t;
    typedef typename solver_t::callback_t                      callback_t;
    typedef typename solver_t::problem_t::function_t::traits_t traits_t;

    typedef typename solver_t::problem_t::function_t           function_t;
    typedef typename function_t::matrix_t                      jacobian_t;
    typedef typename function_t::argument_t                    argument_t;
    typedef typename function_t::const_argument_ref            const_argument_ref;

    typedef GenericDifferentiableFunction<traits_t> differentiableFunction_t;

    /// \brief Requests supported by the logger.
    /// TODO: use strongly typed enum when moving to C++11
    enum LogRequestFlag
      {
	/// \brief Log the argument vector.
	LOG_X = 1 << 0,
	/// \brief Log the cost function's values.
	LOG_COST = 1 << 1,
	/// \brief Log the constraint values.
	LOG_CONSTRAINT = 1 << 2,
	/// \brief Log the constraint Jacobian matrices.
	LOG_CONSTRAINT_JACOBIAN = 1 << 3,
	/// \brief Log the constraint violation.
	LOG_CONSTRAINT_VIOLATION = 1 << 4,
	/// \brief Log the time.
	LOG_TIME = 1 << 5,
	/// \brief Log the solver.
	LOG_SOLVER = 1 << 6
      };
    typedef unsigned int logRequest_t;

    /// \brief Constructor.
    /// \param solver solver that will be logged.
    /// \param path path to the log directory.
    /// \param selfRegister whether the logger will register itself as a
    /// callback with the solver. Set this to false if you use it with a
    /// multiplexer.
    /// \param requests request the logging of specific data.
    explicit OptimizationLogger (solver_t& solver,
				 const boost::filesystem::path& path,
				 bool selfRegister = true,
                                 logRequest_t requests = FullLogging ());

    /// \brief Destructor.
    /// CSV files are written in this destructor.
    virtual ~OptimizationLogger ();

    /// \brief Append extra information to the log file.
    /// \param text text to append.
    void append (const std::string& text);

    /// \brief Append extra information to the log file.
    /// \param text text to append.
    OptimizationLogger<S>& operator<< (const std::string& text);

    /// \brief Append extra information to the log file.
    /// \param u object to print.
    template <typename U>
    OptimizationLogger<S>& operator<< (const U& u);

    /// \brief Log everything.
    static logRequest_t FullLogging ();

    /// \brief Determine if a given request was made by the user.
    ///
    /// \param r request.
    /// \return true if the user made this request.
    bool isRequested (logRequest_t r) const;

  private:
    /// \brief Process constraints in the callback.
    void process_constraints (const typename solver_t::problem_t& pb,
                              const typename solver_t::solverState_t& state,
                              const boost::filesystem::path& iterationPath,
                              const_argument_ref x,
                              value_type& cstrViol);

    /// \brief Attach the logger to the solver.
    void attach ();

    /// \brief Unregister the logger from the solver.
    void unregister ();

  protected:
    void perIterationCallback (const problem_t& pb,
                               solverState_t& state);

    virtual
    void perIterationCallbackUnsafe
    (const typename solver_t::problem_t& pb,
     typename solver_t::solverState_t& state);

  public:
    /// \brief Return the path of the log directory.
    /// \note Seeing that the non-const version of path () needs to be
    /// protected, we need to rename this function to make it public, since
    /// overload resolution takes place before accessibility checks.
    /// \return path of the log directory.
    const boost::filesystem::path& logPath () const;

    /// \brief Display the logger on the specified output stream.
    /// \param o output stream used for display.
    /// \return output stream.
    virtual std::ostream& print (std::ostream& o) const;

  protected:
    /// \brief Return the path of the log directory.
    /// \note This function should not be made public since the non-const
    /// version is protected.
    /// \return path of the log directory.
    const boost::filesystem::path& path () const;

    /// \brief Return the path of the log directory.
    /// This method is protected since streams would need to be updated, and
    /// files that were already written moved to the new location.
    /// \return path of the log directory.
    boost::filesystem::path& path ();

    /// \brief Return the solver associated with the logger.
    /// \return solver associated with the logger.
    const solver_t& solver () const;

    /// \brief Return the solver associated with the logger.
    /// \return solver associated with the logger.
    solver_t& solver ();

    /// \brief Return the callback iteration index.
    /// \return callback iteration index.
    unsigned callbackCallId () const;

  private:
    /// \brief Solver associated with the logger.
    solver_t& solver_;

    /// \brief Path of the logging directory.
    boost::filesystem::path path_;

    /// \brief Output stream for journal.log.
    boost::filesystem::ofstream output_;

    /// \brief Output stream for the argument vector.
    boost::filesystem::ofstream xStream_;

    /// \brief Output stream for the cost function.
    boost::filesystem::ofstream costStream_;

    /// \brief Vector of filenames for the constraint evolutions.
    /// Note: since there can be a lot of constraints, we cannot keep the
    /// streams open, so we store the log paths instead.
    std::vector<boost::filesystem::path> constraintStreamPaths_;

    /// \brief Output stream for the constraint violations.
    boost::filesystem::ofstream constraintViolationStream_;

    /// \brief Callback iteration index.
    unsigned callbackCallId_;

    /// \brief Last time the logger was called (updated at each iteration).
    boost::posix_time::ptime lastTime_;

    /// \brief First time the logger was called.
    boost::posix_time::ptime firstTime_;

    /// \brief Data log requests.
    logRequest_t requests_;

    /// \brief Whether the logger should register itself to the solver.
    /// Note: this provides a way to use the logger in a callback multiplexer.
    bool selfRegister_;

    std::vector<value_type> costs_;
  };
} // end of namespace roboptim

# include <roboptim/core/optimization-logger.hxx>

#endif //! ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HH
