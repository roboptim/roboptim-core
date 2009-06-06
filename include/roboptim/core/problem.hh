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

#ifndef ROBOPTIM_CORE_PROBLEM_HH
# define ROBOPTIM_CORE_PROBLEM_HH
# include <iostream>
# include <stdexcept>
# include <boost/optional.hpp>
# include <boost/static_assert.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Optimization problem.
  ///
  /// An optimization problem is defined as:
  /// - a cost function (\f$\mathbb{R}^n \rightarrow \mathbb{R}\f$)
  /// - one or more constraints functions,
  ///   (\f$\mathbb{R}^n \rightarrow \mathbb{R}^m\f$)
  ///  associated with an interval and a scale,
  /// - a set of intervals and scales for arguments.
  /// .
  ///
  /// The goal of the optimization process is finding a point which
  /// minimizes the cost function and which respects the constraints
  /// (i.e. the result of some functions is inside of specific interval).
  ///
  /// To use the class, one has to instantiate a problem with
  /// a reference to a cost function.
  /// Then, constraints can be added through the addConstraint
  /// method: a reference to a function and an interval is needed.
  ///
  /// The cost function is immutable.
  template <typename F, typename C>
  class Problem
  {
    BOOST_STATIC_ASSERT((boost::is_base_of<Function, F>::value));
  public:
    template <typename F_, typename C_>
    friend class Problem;

    /// \brief Function type.
    typedef F function_t;
    /// \brief Constraint's type.
    typedef C constraint_t;

    // \brief Import function's value_type type.
    typedef typename function_t::value_type value_type;

    /// \brief Constraints are represented as a vector of constraints.
    typedef std::vector<constraint_t> constraints_t;

    /// \brief Optional vector defines a starting point.
    typedef boost::optional<Function::vector_t> startingPoint_t;

    typedef Function::bound_t bound_t;
    typedef Function::bounds_t bounds_t;

    /// \brief Scale vector.
    typedef std::vector<value_type> scales_t;

    /// \pre costfunction \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$
    explicit Problem (const function_t&) throw ();

    /// \brief Copy constructor.
    explicit Problem (const Problem<F, C>&) throw ();

    /// \brief Copy constructor (convert from another class of problem).
    template <typename F_, typename C_>
    explicit Problem (const Problem<F_, C_>&) throw ();

    ~Problem () throw ();

    /// \brief Retrieve cost function.
    /// \return cost function
    const function_t& function () const throw ();

    /// \brief Retrieve constraints.
    /// \return constraints
    const constraints_t& constraints () const throw ();

    /// \brief Add a constraint to the problem.
    /// \param constraint reference to the constraint to add
    /// \param interval interval in which the constraint is satisfied
    /// \param scale constraint scale
    void addConstraint (const C& constraint, interval_t interval,
			value_type scale = 1.)
      throw (std::runtime_error);

    /// \brief Set the initial guess.
    /// \return reference on the initial guess
    startingPoint_t& startingPoint () throw ();

    /// \brief Get the initial guess.
    /// \return reference on the initial guess
    const startingPoint_t& startingPoint () const throw ();

    /// \brief Retrieve constraints bounds.
    /// \return constraints bounds
    const intervals_t& bounds () const throw ();

    /// \brief Retrieve arguments bounds.
    /// Arguments bounds define in which interval
    /// each argument is valid.
    /// \return arguments bounds
    intervals_t& argumentBounds () throw ();

    /// \brief Retrieve arguments bounds.
    /// Arguments bounds define in which interval
    /// each argument is valid.
    /// \return arguments bounds
    const intervals_t& argumentBounds () const throw ();

    /// \brief Retrieve constraints scales.
    /// \return constraints scales
    const scales_t& scales () const throw ();

    /// \brief Retrieve arguments scales.
    /// Arguments scales define which scale is applied for
    /// each argument.
    /// \return arguments scales
    scales_t& argumentScales () throw ();

    /// \brief Retrieve arguments scales.
    /// Arguments scales define which scale is applied for
    /// each argument.
    /// \return arguments scales
    const scales_t& argumentScales () const throw ();

    /// \brief Display the problem on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    std::ostream& print (std::ostream& o) const throw ();

  private:
    /// \brief Objective function.
    const function_t& function_;
    /// \brief Starting point.
    startingPoint_t startingPoint_;
    /// \brief Vector of constraints.
    constraints_t constraints_;

    /// \brief Constraints intervals.
    intervals_t bounds_;
    /// \brief Arguments intervals.
    intervals_t argumentBounds_;
    /// \brief Constraints scale.
    intervals_t scales_;
    /// \brief Arguments' scales.
    intervals_t argumentScales_;
  };

  /// Example shows problem class use.
  /// \example problem-cc.cc

  /// @}

  /// \brief Override operator<< to handle problem display.
  ///
  /// \param o output stream used for display
  /// \param pb problem to be displayed
  /// \return output stream
  template <typename F, typename C>
  std::ostream& operator<< (std::ostream& o, const Problem<F, C>& pb);

} // end of namespace roboptim
# include <roboptim/core/problem.hxx>
#endif //! ROBOPTIM_CORE_PROBLEM_HH
