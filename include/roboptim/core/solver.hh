// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_SOLVER_HH
# define ROBOPTIM_CORE_SOLVER_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <map>
# include <string>

# include <boost/function.hpp>
# include <boost/mpl/assert.hpp>
# include <boost/mpl/logical.hpp>
# include <boost/type_traits/is_base_of.hpp>
# include <boost/variant/variant.hpp>
# include <boost/variant/get.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/function.hh>
# include <roboptim/core/problem.hh>
# include <roboptim/core/generic-solver.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Parameters type.
  struct Parameter
  {
    /// \brief Allowed types for parameters.
    typedef boost::variant<Function::value_type,
			   int, std::string> parameterValues_t;

    /// \brief Parameter description (for humans).
    std::string description;

    /// \brief Value.
    parameterValues_t value;
  };

  /// \brief Solver for a specific problem class.
  ///
  /// This class is parametrized by two types:
  /// the cost function type and the constraints type.
  ///
  /// Solver classes are immutable, the problem can
  /// not be changed after the class instantiation.
  ///
  /// \tparam F cost function type
  /// \tparam C constraints functions type
  /// \pre F is a subtype of Function
  template <typename F, typename C>
  class Solver : public GenericSolver
  {
    BOOST_MPL_ASSERT((boost::mpl::or_<boost::is_base_of<Function, F>,
		      boost::is_base_of<SparseFunction, F> >));
  public:
    /// \brief Solver problem type.
    ///
    /// The solver can solve problems of this type.
    /// If another kind of problem is given, a conversion will be
    /// required.
    typedef Problem<F, C> problem_t;

    /// \brief Import vector type from cost function
    typedef typename F::vector_t vector_t;

    /// \brief Map of parameters.
    typedef std::map<std::string, Parameter> parameters_t;

    /// Per-iteration callback type
    ///
    /// Callback parameters:
    /// \li x is the current considered point,
    /// \li problem is a (constant) reference to the problem
    ///
    typedef boost::function<void (const vector_t& x,
				  const problem_t& problem)> callback_t;


    /// \brief Instantiate a solver from a problem.
    ///
    /// \param problem problem that should be solved
    explicit Solver (const problem_t& problem) throw ();


    /// \brief Instantiate a solver from a problem in a different problem class.
    ///
    /// This constructor is called when the problem cost function or/and
    /// constraints type does not match solver's types.
    ///
    /// This is only possible if the problem provides too much information
    /// compared to the solver requirements:
    /// if the problem contains twice derivable function and the solver requires
    /// only derivable function, it will work however the opposite will fail.
    /// Problem compatibility is known at compile-time, so the failure will be
    /// at compile-time.
    ///
    /// \tparam F_ original cost function type
    /// \tparam C_ original constraints functions type
    /// \param problem problem that should be solved
    template <typename F_, typename C_>
    explicit Solver (const Problem<F_, C_>& problem) throw ();

    virtual ~Solver () throw ();

    /// \brief Retrieve the problem.
    /// \return problem this solver is solving
    const problem_t& problem () const throw ();

    /// \name Parameters
    /// \{
    const parameters_t& parameters () const throw ();
    parameters_t& parameters () throw ();

    template <typename T>
    const T& getParameter (const std::string& key) const;
    /// \}

    /// \brief Set the per-iteration callback.
    ///
    /// The per-iteration callback is a callback called each time one
    /// iteration of the optimization process is finished.
    ///
    /// Not all the solvers support such a callback so this method may
    /// throw a std::runtime_error to let you know this feature is
    /// unsupported.
    virtual void
    setIterationCallback (callback_t /*callback*/) throw (std::runtime_error)
    {
      throw std::runtime_error
	("iteration callback is not supported by this solver");
    }

    /// \brief Display the solver on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();
  protected:
    /// \brief Problem that will be solved.
    const problem_t problem_;

    /// \brief Solver parameters (run-time configuration).
    parameters_t parameters_;

    /// \brief Pointer to function logger (see log4cxx documentation).
    static log4cxx::LoggerPtr logger;
  };

  /// \brief Override operator<< to display ``parameters'' objects.
  ///
  /// \param o output stream used for display
  /// \param ns NoSolution object, ignored
  /// \return output stream
  ROBOPTIM_DLLAPI std::ostream&
  operator<< (std::ostream& o, const Parameter& parameter);
  /// @}

} // end of namespace roboptim
# include <roboptim/core/solver.hxx>
#endif //! ROBOPTIM_CORE_SOLVER_HH
