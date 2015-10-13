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
# include <boost/mpl/vector.hpp>
# include <boost/type_traits/is_same.hpp>

# include <roboptim/core/config.hh>

namespace roboptim
{
  /// \brief Log the optimization process (values, Jacobians, time taken
  /// etc.).
  /// \tparam T solver type.
  template <typename T>
  class OptimizationLogger
  {
  public:
    typedef T solver_t;
    typedef typename solver_t::problem_t                       problem_t;
    typedef typename solver_t::problem_t::value_type           value_type;
    typedef typename solver_t::problem_t::size_type            size_type;
    typedef typename solver_t::problem_t::vector_t             vector_t;
    typedef typename solver_t::solverState_t                   solverState_t;
    typedef typename solver_t::callback_t                      callback_t;
    typedef typename solver_t::problem_t::function_t::traits_t traits_t;

    typedef typename solver_t::problem_t::function_t           function_t;
    typedef typename function_t::matrix_t                      jacobian_t;
    typedef typename function_t::const_argument_ref            const_argument_ref;

    typedef GenericDifferentiableFunction<traits_t> differentiableFunction_t;

    /// \brief Constructor.
    /// \param solver solver that will be logged.
    /// \param path path to the log directory.
    /// \param selfRegister whether the logger will register itself as a
    /// callback with the solver. Set this to false if you use it with a
    /// multiplexer.
    explicit OptimizationLogger (solver_t& solver,
				 const boost::filesystem::path& path,
				 bool selfRegister = true);

    /// \brief Destructor.
    /// CSV files are written in this destructor.
    virtual ~OptimizationLogger ();

    /// \brief Append extra information to the log file.
    /// \param text text to append.
    void append (const std::string& text);

    /// \brief Return the callback function.
    /// This can be used with a callback multiplexer.
    /// \return callback function.
    callback_t callback ();

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
                               const solverState_t& state);

    virtual
    void perIterationCallbackUnsafe
    (const typename solver_t::problem_t& pb,
     const typename solver_t::solverState_t& state);

  public:
    /// \brief Return the path of the log directory.
    /// \note Seeing that the non-const version of path () needs to be
    /// protected, we need to rename this function to make it public, since
    /// overload resolution takes place before accessibility checks.
    /// \return path of the log directory.
    const boost::filesystem::path& logPath () const;

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

    /// \brief Callback iteration index.
    unsigned callbackCallId_;

    /// \brief Last time the logger was called (updated at each iteration).
    boost::posix_time::ptime lastTime_;

    /// \brief First time the logger was called.
    boost::posix_time::ptime firstTime_;

    /// \brief Whether the logger should register itself to the solver.
    /// Note: this provides a way to use the logger in a callback multiplexer.
    bool selfRegister_;

    std::vector<vector_t> x_;
    std::vector<value_type> costs_;
    std::vector<value_type> constraintViolations_;
    std::vector<std::vector<vector_t> > constraints_;
  };
} // end of namespace roboptim

# include <roboptim/core/optimization-logger.hxx>

#endif //! ROBOPTIM_CORE_OPTIMIZATION_LOGGER_HH
