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
 * \file solver.hh
 *
 * \brief Declaration of the Solver class.
 */

#ifndef OPTIMIZATION_SOLVER_HH
# define OPTIMIZATION_SOLVER_HH
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
    enum solutions {
      SOLVER_NO_SOLUTION,
      SOLVER_VALUE,
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
    /// \}


    /// \{
    /// FIXME: clean constraints.
    struct InequalityConstraint
    {
      size_type index;
      value_type lower;
      value_type upper;
    };
    struct FunctionConstraint
    {
      FunctionConstraint (function_t fct)
        : function_ (fct)
      {}

      function_t function_;
      value_type lower;
      value_type upper;
    };
    typedef boost::variant<InequalityConstraint, FunctionConstraint> constraint_t;
    typedef std::vector<constraint_t> constraints_t;
    /// \}

    /// \{
    explicit Solver (function_t, size_type, gradient_t) throw ();
    virtual ~Solver () throw ();
    /// \}

    void reset () throw ();

    void setStartingPoint (const array_t&) throw ();

    virtual result_t getMinimum () throw () = 0;

    function_t getFunction () const throw ();
    gradient_t getGradient () const throw ();
    std::size_t getArity () const throw ();
    constraints_t& getConstraints () throw ();
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
