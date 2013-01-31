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
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <iostream>
# include <stdexcept>

# include <boost/mpl/transform.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/optional.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/static_assert.hpp>
# include <boost/type_traits/is_base_of.hpp>
# include <boost/variant.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/function.hh>

namespace roboptim
{
  namespace detail
  {
    using namespace boost;
    using namespace boost::mpl;

    /// \brief Transform a types list into a types list of shared pointers.
    ///
    /// If the input list is:
    /// \code
    /// boost::mpl::vector<int, long>
    /// \endcode
    ////
    /// then the result (type) will be:
    /// \code
    /// boost::mpl::vector<boost::shared_ptr<int>,
    ///                    boost::shared_ptr<long> >
    /// \endcode
    ///
    /// \tparam CLIST list that will be transformed
    template <typename CLIST>
    struct add_shared_ptr
    {
      /// \brief Result.
      typedef typename boost::mpl::transform
      <CLIST, typename boost::shared_ptr<boost::mpl::_1> >::type type;
    };
  } // end of namespace detail.


  /// \addtogroup roboptim_problem
  /// @{
  /// \brief Optimization problem without constraints.
  ///
  /// An optimization problem is defined as:
  /// - a cost function (\f$\mathbb{R}^n \rightarrow \mathbb{R}\f$)
  /// - a set of intervals and scales for arguments.
  /// .
  ///
  /// The goal of the optimization process is finding a point which
  /// minimizes the cost function
  ///
  /// To use the class, one has to instantiate a problem with
  /// a reference to a cost function.
  /// method: a reference to a function and an interval is needed.
  ///
  /// The cost function is immutable.
  ///
  /// Unlike other classes which just copy functions, pointers are used
  /// here in order to allow sub-classes of constraints to be inserted
  /// in the problem.
  /// For instance, a twice derivable function can be inserted in
  /// a problem which expects a derivable function.
  ///
  /// \tparam F function type
  template <typename F>
  class Problem <F, boost::mpl::vector<> >
  {
    BOOST_STATIC_ASSERT((boost::is_base_of<Function, F>::value));

    //FIXME: check that CLIST is a MPL vector of Function's sub-classes.
  public:
    template <typename F_, typename CLIST_>
    friend class Problem;

    /// \brief Function type.
    ///
    /// This has to be either Function or one of its
    /// sub-classes.
    typedef F function_t;

    // \brief Import function's value_type type.
    typedef typename function_t::value_type value_type;

    /// \brief Optional vector defines a starting point.
    typedef boost::optional<Function::vector_t> startingPoint_t;

    typedef Function::interval_t interval_t;
    typedef Function::intervals_t intervals_t;

    /// \brief Scale vector.
    typedef std::vector<value_type> scales_t;

    /// \name Constructors and destructors.
    /// \{

    /// \pre costfunction \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$
    explicit Problem (const function_t&) throw ();

    /// \brief Copy constructor.
    explicit Problem (const Problem<F, boost::mpl::vector<> >&) throw ();

    /// \brief Copy constructor (convert from another class of problem).
    template <typename F_>
    explicit Problem (const Problem<F_, boost::mpl::vector<> >&) throw ();

    ~Problem () throw ();

    /// \}


    /// \name Cost function.
    /// \{

    /// \brief Retrieve cost function.
    /// \return cost function
    const function_t& function () const throw ();

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

    /// \}

    /// \name Starting point (initial guess).
    /// \{

    /// \brief Set the initial guess.
    /// \return reference on the initial guess
    startingPoint_t& startingPoint () throw ();

    /// \brief Get the initial guess.
    /// \return reference on the initial guess
    const startingPoint_t& startingPoint () const throw ();

    /// \}


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

    /// \brief Arguments intervals.
    intervals_t argumentBounds_;
    /// \brief Arguments' scales.
    scales_t argumentScales_;
  };


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
  ///
  /// Constraints are stored as a Boost.Variant of smart pointers
  /// (i.e. more precisely using a boost::shared_ptr) representing all
  /// the different possibles constraint types.
  ///
  /// It is recommended to add a constraint using the following syntax:
  /// \code
  /// problem.addConstraint<C> (boost::make_shared<MyFunction> (...), ...);
  /// \endcode
  /// where C is the constraint type and MyFunction the function type.
  ///
  ///
  /// Unlike other classes which just copy functions, pointers are used
  /// here in order to allow sub-classes of constraints to be inserted
  /// in the problem.
  /// For instance, a twice derivable function can be inserted in
  /// a problem which expects a derivable function.
  ///
  /// \tparam F function type
  /// \tparam CLIST type list satisfying MPL's sequence concept
  template <typename F, typename CLIST>
  class Problem
  {
    BOOST_STATIC_ASSERT((boost::is_base_of<Function, F>::value));

    //FIXME: check that CLIST is a MPL vector of Function's sub-classes.
  public:
    template <typename F_, typename CLIST_>
    friend class Problem;

    /// \brief Function type.
    ///
    /// This has to be either Function or one of its
    /// sub-classes.
    typedef F function_t;

    /// \brief Constraint's type.
    ///
    /// Generate a Boost.Variant of shared pointers tyle from the
    /// static constraints types list.
    ///
    /// For instance, if one instantiates
    /// \code
    ///  Problem<QuadraticFunction, vector<LinearFunction, QuadraticFunction> >
    /// \endcode
    /// then this type will be set to:
    /// \code
    /// boost::variant<boost::shared_ptr<LinearFunction>,
    ///                boost::shared_ptr<QuadraticFunction> >
    /// \endcode
    ///
    /// The meta-algorithm which add shared pointers is implemented
    /// in detail::add_shared_pointer.
    typedef typename boost::make_variant_over
    <typename detail::add_shared_ptr<CLIST>::type>::type constraint_t;

    // \brief Import function's value_type type.
    typedef typename function_t::value_type value_type;

    /// \brief Constraints are represented as a vector of constraints.
    typedef std::vector<constraint_t> constraints_t;

    /// \brief Optional vector defines a starting point.
    typedef boost::optional<Function::vector_t> startingPoint_t;

    typedef Function::interval_t interval_t;
    typedef Function::intervals_t intervals_t;

    /// \brief Scale vector.
    typedef std::vector<value_type> scales_t;

    /// \brief Vector of interval vectors. This type is used to take
    /// into account the fact that constraints can have output values
    /// in \f$\mathbb{C}^{m}\f$.
    ///
    /// If \f$m=1\f$, then the associated interval vector contains
    /// only one element of interval_t type.
    typedef std::vector<intervals_t> intervalsVect_t;

    /// \brief Vector of scale vectors. This type is used to take into
    /// account the fact that constraints can have output values in
    /// \f$\mathbb{R}^m\f$.
    ///
    /// If \f$m=1\f$, then the associated scale vector contains only
    /// one element of scale_t type.
    typedef std::vector<scales_t> scalesVect_t;

    /// \name Constructors and destructors.
    /// \{

    /// \pre costfunction \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$
    explicit Problem (const function_t&) throw ();

    /// \brief Copy constructor.
    explicit Problem (const Problem<F, CLIST>&) throw ();

    /// \brief Copy constructor (convert from another class of problem).
    template <typename F_, typename CLIST_>
    explicit Problem (const Problem<F_, CLIST_>&) throw ();

    ~Problem () throw ();

    /// \}


    /// \name Cost function.
    /// \{

    /// \brief Retrieve cost function.
    /// \return cost function
    const function_t& function () const throw ();

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

    /// \}


    /// \name Constraints.
    /// \{

    /// \brief Retrieve constraints.
    /// \return constraints
    const constraints_t& constraints () const throw ();

    /// \brief Add a constraint to the problem.
    ///
    /// Useful only when \f$m=1\f$, use addConstraint
    /// (boost::shared_ptr<C> constraint, intervals_t intervals,
    /// scales_t scales) instead.
    ///
    /// \param constraint the constraint that will be added
    /// \param interval interval in which the constraint is satisfied
    /// \param scale constraint scale
    /// \tparam C constraint type (has to be in CLIST)
    template <typename C>
    void addConstraint (boost::shared_ptr<C> constraint,
			interval_t interval,
			value_type scale = 1.)
      throw (std::runtime_error);

    /// \brief Add a constraint to the problem.
    ///
    /// The constraint can be multidimensional.
    ///
    /// \param constraint the constraint that will be added
    /// \param intervals interval vector in which the constraint
    /// is satisfied
    /// \param scale constraint scale
    /// \tparam C constraint type (has to be in CLIST)
    template <typename C>
    void addConstraint (boost::shared_ptr<C> constraint,
			intervals_t intervals,
			scales_t scales)
      throw (std::runtime_error);

    /// \brief Retrieve constraints bounds vector.
    /// \return constraints bounds vector
    const intervalsVect_t& boundsVector () const throw ();

    /// \brief Retrieve constraints scales vector.
    /// \return constraints scales vector
    const scalesVect_t& scalesVector () const throw ();

    /// \}


    /// \name Starting point (initial guess).
    /// \{

    /// \brief Set the initial guess.
    /// \return reference on the initial guess
    startingPoint_t& startingPoint () throw ();

    /// \brief Get the initial guess.
    /// \return reference on the initial guess
    const startingPoint_t& startingPoint () const throw ();

    /// \}


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

    /// \brief Constraints intervals vector.
    intervalsVect_t boundsVect_;

    /// \brief Arguments intervals.
    intervals_t argumentBounds_;

    /// \brief Constraints scales vector.
    scalesVect_t scalesVect_;

    /// \brief Arguments' scales.
    scales_t argumentScales_;

  };

  /// Example shows problem class use.
  /// \example problem-cc.cc

  /// @}

  /// \brief Override operator<< to handle problem display.
  ///
  /// \param o output stream used for display
  /// \param pb problem to be displayed
  /// \return output stream
  template <typename F>
  std::ostream& operator<< (std::ostream& o,
			    const Problem<F, boost::mpl::vector<> >& pb);

  /// \brief Override operator<< to handle problem display.
  ///
  /// \param o output stream used for display
  /// \param pb problem to be displayed
  /// \return output stream
  template <typename F, typename CLIST>
  std::ostream& operator<< (std::ostream& o, const Problem<F, CLIST>& pb);

} // end of namespace roboptim
# include <roboptim/core/problem.hxx>
#endif //! ROBOPTIM_CORE_PROBLEM_HH
