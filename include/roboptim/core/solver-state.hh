// Copyright (C) 2013 by Benjamin Chretien, CNRS.
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

#ifndef ROBOPTIM_CORE_SOLVER_STATE_HH
# define ROBOPTIM_CORE_SOLVER_STATE_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <map>
# include <string>

# include <boost/variant/variant.hpp>
# include <boost/variant/get.hpp>
# include <boost/optional.hpp>
# include <boost/noncopyable.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/function.hh>
# include <roboptim/core/problem.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Solver state parameters type.
  ///
  /// \tparam F function type.
  template <typename F>
  struct StateParameter
  {
    typedef typename F::value_type value_type;
    typedef typename F::vector_t vector_t;

    /// \brief Allowed types for parameters:
    ///   - value (e.g. constraint violation)
    ///   - vector (e.g. current x)
    ///   - integer (e.g. current iteration)
    ///   - string (e.g. warning message from the solver)
    ///   - bool (e.g. whether to stop the optimization)
    typedef boost::variant<value_type, vector_t,
                           int, std::string, bool> stateParameterValues_t;

    /// \brief Display the solver parameter on the specified output stream.
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

    /// \brief Parameter description (for humans).
    std::string description;

    /// \brief Value.
    stateParameterValues_t value;
  };

  /// \brief State of the solver.
  ///
  ///
  /// \tparam P problem type.
  template <typename P>
  class SolverState : public boost::noncopyable
  {
  public:
    /// \brief Problem type
    typedef P problem_t;

    /// \brief Import vector type from problem
    typedef typename P::vector_t vector_t;

    /// \brief Import value type from problem
    typedef typename P::value_type value_type;

    /// \brief Import function type from problem
    typedef typename P::function_t function_t;

    /// \brief Map of parameters.
    typedef std::map<std::string, StateParameter<function_t> > parameters_t;

    /// \brief Instantiate a solver from a problem.
    ///
    /// \param problem problem that should be solved
    explicit SolverState (const problem_t& problem);

    virtual ~SolverState ();

    /// \brief Retrieve the current optimization parameters.
    /// \return current optimization parameters
    const vector_t& x () const;
    vector_t& x ();

    /// \brief Retrieve the current cost.
    /// \return current cost
    const boost::optional<value_type>& cost () const;
    boost::optional<value_type>& cost ();

    /// \brief Retrieve the current constraint violation.
    /// \return current constraint violation
    const boost::optional<value_type>& constraintViolation () const;
    boost::optional<value_type>& constraintViolation ();

    /// \name Parameters
    /// \{
    const parameters_t& parameters () const;
    parameters_t& parameters ();

    /// \brief Get a parameter.
    /// \tparam T parameter type.
    /// \param key parameter key.
    /// \return parameter.
    /// \throw std::out_of_range
    template <typename T>
    const T& getParameter (const std::string& key) const;

    /// \brief Get a parameter.
    /// \tparam T parameter type.
    /// \param key parameter key.
    /// \return parameter.
    /// \throw std::out_of_range
    template <typename T>
    T& getParameter (const std::string& key);
    /// \}


    /// \brief Display the solver state on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

  protected:
    /// \brief Current optimization parameters.
    vector_t x_;

    /// \brief Current cost.
    /// The solver may not provide the cost at each iteration, hence the use
    /// of boost::optional.
    boost::optional<value_type> cost_;

    /// \brief Current constraint violation.
    /// The solver may not provide the constraint violation at each iteration,
    /// hence the use of boost::optional.
    boost::optional<value_type> constraintViolation_;

    /// \brief Solver state extra parameters (solver-specific parameters etc.).
    parameters_t parameters_;
  };

  /// \brief Override operator<< to display ``parameters'' objects.
  ///
  /// \param o output stream used for display
  /// \param parameter parameter to display
  /// \tparam F function type.
  /// \return output stream
  template <typename F>
  std::ostream&
  operator<< (std::ostream& o, const StateParameter<F>& parameter);

  /// \brief Override operator<< to display ``parameters'' objects.
  ///
  /// \param o output stream used for display
  /// \param state solver state to display
  /// \tparam P problem type.
  /// \return output stream
  template <typename P>
  std::ostream&
  operator<< (std::ostream& o, const SolverState<P>& state);
  /// @}

} // end of namespace roboptim
# include <roboptim/core/solver-state.hxx>
#endif //! ROBOPTIM_CORE_SOLVER_STATE_HH
