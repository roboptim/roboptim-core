// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.

/**
 * \file src/solver.hh
 *
 * \brief Declaration of the Solver class.
 */

#ifndef OPTIMIZATION_SOLVER_HH
# define OPTIMIZATION_SOLVER_HH
# include <limits>
# include <stdexcept>
# include <utility>
# include <vector>

# include <boost/function.hpp>
# include <boost/noncopyable.hpp>
# include <boost/numeric/ublas/matrix.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/optional/optional.hpp>
# include <boost/variant/variant.hpp>

# include <optimization-fwd.hh>

namespace optimization
{
  namespace ublas = boost::numeric::ublas;

  /// \brief Base error.
  /// All other errors inherits this class.
  struct SolverError : public std::runtime_error
  {
    SolverError (const std::string& arg) throw ()
      : std::runtime_error (arg)
    {}
  };

  /// \brief Returned by Solver::getMinimum if no solution can be
  /// found (but no error has been encountered during the process).
  struct NoSolution {};

  /// Generic solver class.
  class Solver : public boost::noncopyable
  {
  public:
    /// Define the kind of solution which has been found.
    enum solutions {
      /// Solution has yet to be found.
      SOLVER_NO_SOLUTION,
      /// Solution has been found.
      SOLVER_VALUE,
      /// The solver failed to found a solution.
      SOLVER_ERROR
    };

    /// \defgroup Typedefs.
    /// \{

    /// Data type.
    typedef double value_type;
    /// Size type.
    typedef std::size_t size_type;

    /// Array type.
    typedef ublas::vector<value_type> array_t;
    /// Matrix type.
    typedef ublas::matrix<value_type> matrix_t;

    /// Function type.
    typedef boost::function<value_type (const array_t&)> function_t;
    /// Result type.
    typedef boost::variant<NoSolution, array_t, SolverError> result_t;

    /// Gradient type.
    typedef boost::function<const array_t (const array_t&)> gradient_t;

    /// Bound type (lower bound, upper bound).
    typedef std::pair<value_type, value_type> bound_t;
    /// Bound vector.
    typedef ublas::vector<bound_t> bounds_t;
    /// \}


    /// \defgroup Constraints related types.
    /// \{

    /// Function constraint such that \f$lower < function (x) < upper\f$.
    struct Constraint
    {
      /// Construct a new constraint from a function, a lower and upper bound.
      /// \param fct Constraint function.
      /// \param l Lower bound (defaulting to \f$-\infty\f$).
      /// \param u Upper bound (defaulting to \f$+\infty\f$).
      Constraint (function_t fct,
                  value_type l = -std::numeric_limits<value_type>::infinity (),
                  value_type u = std::numeric_limits<value_type>::infinity ())
        : function (fct),
          lower (l),
          upper (u)
      {}

      /// Default copy constructor.
      Constraint* operator= (const Constraint& c)
      {
        return new Constraint (c.function, c.lower, c.upper);
      }

      /// Constraint function.
      function_t function;
      /// Lower bound.
      value_type lower;
      /// Upper bound.
      value_type upper;
    };
    /// A problem can have several constraints.
    typedef std::vector<Constraint> constraints_t;
    /// \}

    /// Hessian function.
    typedef boost::function<
      const matrix_t (const array_t&, const constraints_t&,
                      value_type, const array_t&)> hessian_t;
    /// Jacobian of the lagrangian function.
    typedef boost::function<
      const matrix_t (const array_t&, const constraints_t&)> jacobian_t;

    /// \defgroup Constructors and destructors.
    /// \{

    /// Main constructor.
    /// \param fct Objective function
    /// \param n Problem's size.
    /// \param g Gradient function.
    /// \param h Hessian function.
    /// \param j Jacobian's lagrangian function.
    explicit Solver (function_t fct,
                     size_type n,
                     gradient_t g = gradient_t (),
                     hessian_t h = hessian_t (),
                     jacobian_t j = jacobian_t ()) throw ();
    /// Destructor.
    virtual ~Solver () throw ();

    /// \}

    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum is called.
    void reset () throw ();

    /// Set the (optional) starting point.
    void setStartingPoint (const array_t&) throw ();

    /// Returns the function minimum (and solve the problem, if
    /// it has not yet been solved).
    virtual result_t getMinimum () throw () = 0;

    /// Get the objective function.
    function_t getFunction () const throw ();
    /// Get the gradient of the objective function.
    gradient_t getGradient () const throw ();
    /// Get the Hessian
    hessian_t getHessian () const throw ();
    /// Get the Jacobian
    jacobian_t getJacobian () const throw ();
    /// Get problem arity.
    size_type getArity () const throw ();

    /// Get bound on a specific variable.
    bound_t getBound(size_type) const throw ();
    /// Set bound on a specific variable.
    void setBound(size_type, bound_t) throw ();

    /// Get constraints list.
    constraints_t& getConstraints () throw ();
    /// Get constraints list.
    const constraints_t& getConstraints () const throw ();

  protected:
    /// Number of arguments in objective function.
    size_type arity_;
    /// Function to minimize.
    function_t function_;
    /// Gradient function.
    gradient_t gradient_;
    /// Starting point.
    boost::optional<array_t> start_;
    /// Variables bounds.
    bounds_t bounds_;
    /// Constraints
    constraints_t constraints_;

    /// Hessian
    hessian_t hessian_;
    /// Jacobian
    jacobian_t jacobian_;

    /// Result of minimization.
    result_t result_;


  };

} // end of namespace optimization

#endif //! OPTIMIZATION_SOLVER_HH
