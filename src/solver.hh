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
# include <utility>
# include <vector>

# include <boost/function.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/optional.hpp>
# include <boost/variant.hpp>
# include <boost/utility.hpp>


# include <optimization-fwd.hh>

namespace optimization
{
  namespace ublas = boost::numeric::ublas;

  /// Generic solver error.
  struct SolverError
  {
    //FIXME: make exception.
  };

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

    /// \{
    /// Data type.
    typedef double value_type;
    /// Size type.
    typedef std::size_t size_type;

    /// Array type.
    typedef ublas::vector<value_type> array_t;
    /// Function type.
    typedef const boost::function<value_type (const array_t&)>& function_t;
    /// Result type.
    typedef boost::variant<NoSolution, array_t, SolverError> result_t;

    /// Gradient type.
    typedef boost::optional<
      const boost::function<const array_t (const array_t&)>&> gradient_t;

    /// Bound type (lower bound, upper bound).
    typedef std::pair<value_type, value_type> bound_t;
    /// Bound vector.
    typedef ublas::vector<bound_t> bounds_t;
    /// \}


    /// \{
    // FIXME: linear/non-linear constraint?

    /// Function constraint such that
    /// lower < function_ (x) < upper
    struct FunctionConstraint
    {
      FunctionConstraint (function_t fct)
        : function_ (fct)
      {}

      function_t function_;
      value_type lower;
      value_type upper;
    };
    /// A problem can have several constraints.
    typedef std::vector<FunctionConstraint> constraints_t;
    /// \}

    /// \{
    explicit Solver (function_t, size_type, gradient_t) throw ();
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
    /// Get problem arity.
    std::size_t getArity () const throw ();

    /// Get bound on a specific variable.
    bound_t getBound(size_t) const throw ();
    /// Set bound on a specific variable.
    void setBound(size_t, bound_t) throw ();

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

    /// Result of minimization.
    result_t result_;


    //FIXME: More?
    //boost::optional<jacobian_t> jacobian_;
    //boost::optional<lagrangian_t> lagrangian_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_SOLVER_HH
