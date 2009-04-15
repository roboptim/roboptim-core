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
 * \brief Declaration of the Problem class.
 */

#ifndef OPTIMIZATION_PROBLEM_HH
# define OPTIMIZATION_PROBLEM_HH
# include <iostream>
# include <stdexcept>
# include <boost/optional.hpp>
# include <boost/static_assert.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <liboptimization/fwd.hh>
# include <liboptimization/function.hh>

namespace optimization
{
  /// Define a problem in a generic way.
  template <typename F, typename C>
  class Problem
  {
    BOOST_STATIC_ASSERT((boost::is_base_of<Function, F>::value));
  public:
    template <typename F_, typename C_>
    friend class Problem;

    /// Function type.
    typedef F function_t;
    /// Constraint's type.
    typedef C constraint_t;

    // Import function's value_type type.
    typedef typename function_t::value_type value_type;

    /// Constraints are represented as a vector of constraints.
    typedef std::vector<constraint_t> constraints_t;
    /// Optional vector defines a starting point.
    typedef boost::optional<Function::vector_t> startingPoint_t;

    /// Bound type (lower, upper).
    /// Use -infinity / +infinity to disable a bound.
    typedef std::pair<value_type, value_type> bound_t;
    /// Vector of bound.
    typedef std::vector<bound_t> bounds_t;
    /// Scale vector.
    typedef std::vector<value_type> scales_t;

    explicit Problem (const function_t&) throw ();

    /// Copy constructor.
    explicit Problem (const Problem<F, C>&) throw ();

    /// Copy constructor (convert from another class of problem).
    template <typename F_, typename C_>
    explicit Problem (const Problem<F_, C_>&) throw ();

    ~Problem () throw ();

    const function_t& function () const throw ();

    const constraints_t& constraints () const throw ();

    void addConstraint (const C&, value_type = 1.)
      throw (std::runtime_error);
    void addConstraint (const C&, bound_t, value_type = 1.)
      throw (std::runtime_error);

    startingPoint_t& startingPoint () throw ();
    const startingPoint_t& startingPoint () const throw ();

    const bounds_t& bounds () const throw ();
    bounds_t& argBounds () throw ();
    const bounds_t& argBounds () const throw ();

    const scales_t& scales () const throw ();
    scales_t& argScales () throw ();
    const scales_t& argScales () const throw ();

    std::ostream& print (std::ostream& o) const throw ();

    /// Construct a bound from a lower and upper bound.
    static bound_t makeBound (value_type l, value_type u) throw ()
    {
      return std::make_pair (l, u);
    }

    /// Construct an infinite bound.
    static bound_t makeInfiniteBound () throw ()
    {
      return std::make_pair (-Function::infinity (), Function::infinity  ());
    }

    /// Construct a bound from a lower bound.
    static bound_t makeLowerBound (value_type u) throw ()
    {
      return makeBound (-Function::infinity  (), u);
    }

    /// Construct a bound from an upper bound.
    static bound_t makeUpperBound (value_type l) throw ()
    {
      return makeBound (l, Function::infinity  ());
    }

  private:
    // Objective function.
    const function_t& function_;
    // Starting point.
    startingPoint_t startingPoint_;
    // Vector of constraints.
    constraints_t constraints_;

    /// Constraints bounds.
    bounds_t bounds_;
    /// Arguments bounds.
    bounds_t argBounds_;
    /// Constraints scale.
    scales_t scales_;
    /// Arguments' scales.
    scales_t argScales_;
  };

  template <typename F, typename C>
  std::ostream& operator<< (std::ostream& o, const Problem<F, C>& pb);
} // end of namespace optimization

# include <liboptimization/problem.hxx>
#endif //! OPTIMIZATION_PROBLEM_HH
