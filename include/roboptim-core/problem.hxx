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

/**
 * \brief Implementation of the Problem class.
 */

#ifndef ROBOPTIM_CORE_PROBLEM_HXX
# define ROBOPTIM_CORE_PROBLEM_HXX
# include <algorithm>
# include <boost/numeric/ublas/io.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/variant.hpp>
# include <boost/variant/get.hpp>
# include <boost/variant/apply_visitor.hpp>

# include <roboptim-core/indent.hh>
# include <roboptim-core/util.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_problem
     @{
  */

  template <typename F, typename C>
  Problem<F, C>::Problem (const function_t& f) throw ()
    : function_ (f),
      startingPoint_ (),
      constraints_ (),
      bounds_ (),
      argBounds_ (f.n),
      scales_ (),
      argScales_ (f.n)
  {
    // Check that in the objective function m = 1 (R^n -> R).
    assert (f.m == 1);

    // Initialize bound.
    std::fill (argBounds_.begin (), argBounds_.end (), makeInfiniteBound ());
    // Initialize scale.
    std::fill (argScales_.begin (), argScales_.end (), 1.);
  }

  template <typename F, typename C>
  Problem<F, C>::~Problem () throw ()
  {
  }

  // Copy constructor.
  template <typename F, typename C>
  Problem<F, C>::Problem (const Problem<F, C>& pb) throw ()
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (pb.constraints_),
      bounds_ (pb.bounds_),
      argBounds_ (pb.argBounds_),
      scales_ (pb.scales_),
      argScales_ (pb.argScales_)
  {
  }

  // Copy constructor (convert from another class of problem).
  template <typename F, typename C>
  template <typename F_, typename C_>
  Problem<F, C>::Problem (const Problem<F_, C_>& pb) throw ()
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ (),
      bounds_ (pb.bounds_),
      argBounds_ (pb.argBounds_),
      scales_ (pb.scales_),
      argScales_ (pb.argScales_)
  {
    // Check that F is a subtype of F_.
    BOOST_STATIC_ASSERT((boost::is_base_of<F, F_>::value));

    // Check that C is a subtype of C_.
    typedef typename boost::remove_pointer<C_>::type rpC_;
    typedef typename boost::remove_pointer<C>::type rpC;
    BOOST_STATIC_ASSERT((boost::is_base_of<rpC, rpC_>::value));

    std::copy (pb.constraints_.begin (), pb.constraints_.end (),
               constraints_.begin ());
  }

  template <typename F, typename C>
  const typename Problem<F, C>::function_t&
  Problem<F, C>::function () const throw ()
  {
    return function_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::constraints_t&
  Problem<F, C>::constraints () const throw ()
  {
    return constraints_;
  }

  template <typename F, typename C>
  void
  Problem<F, C>::addConstraint (const C& x, value_type s)
    throw (std::runtime_error)
  {
    addConstraint (x, makeInfiniteBound (), s);
  }

  template <typename F, typename C>
  void
  Problem<F, C>::addConstraint (const C& x, bound_t b, value_type s)
    throw (std::runtime_error)
  {
    constraints_.push_back (x);
    bounds_.push_back (b);
    scales_.push_back (s);
  }

  template <typename F, typename C>
  typename Problem<F, C>::startingPoint_t&
  Problem<F, C>::startingPoint () throw ()
  {
    return startingPoint_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::startingPoint_t&
  Problem<F, C>::startingPoint () const throw ()
  {
    return startingPoint_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::bounds_t&
  Problem<F, C>::bounds () const throw ()
  {
    return bounds_;
  }

  template <typename F, typename C>
  typename Problem<F, C>::bounds_t&
  Problem<F, C>::argBounds () throw ()
  {
    return argBounds_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::bounds_t&
  Problem<F, C>::argBounds () const throw ()
  {
    return argBounds_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::scales_t&
  Problem<F, C>::scales () const throw ()
  {
    return scales_;
  }

  template <typename F, typename C>
  typename Problem<F, C>::scales_t&
  Problem<F, C>::argScales () throw ()
  {
    return argScales_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::scales_t&
  Problem<F, C>::argScales () const throw ()
  {
    return argScales_;
  }


  namespace detail
  {
    template <typename T>
    std::ostream&
    impl_print (std::ostream& o, const T* t)
    {
      return o << *t;
    }

    template <typename T>
    std::ostream&
    impl_print (std::ostream& o, const T& t)
    {
      return o << t;
    }
  }

  template <typename F, typename C>
  std::ostream&
  Problem<F, C>::print (std::ostream& o) const throw ()
  {
    o << "Problem:" << incendl;
    // Function.
    o << function () << iendl;

    // Arguments' bounds.
    o << "Argument's bounds: " << argBounds () << iendl;
    // Arguments' scales.
    o << "Argument's scales: " << argScales () << iendl;

    // Constraints.
    if (constraints ().empty ())
      o << "No constraints.";
    else
      o << "Number of constraints: " << constraints ().size ();

    typedef typename constraints_t::const_iterator citer_t;
    unsigned i = 0;
    for (citer_t it = constraints ().begin ();
         it != constraints ().end ();
         ++it, ++i)
      {
        o << iendl << incindent
          << "Constraint " << i << incindent << iendl
          << "Bounds: " << bounds ()[i] << iendl
          << "Scales: " << scales ()[i] << iendl
          << decindent << decindent;
      }

    // Starting point.
    if (startingPoint_)
      o << iendl << "Starting point: " << *startingPoint_;
    else
      o << iendl << "No starting point.";

    // Infinity.
    o << iendl << "Infinity value (for all functions): " << Function::infinity ();

    // Arguments' bounds.
    // Arguments' scales.


    return o << decindent;
  }

  template <typename F, typename C>
  std::ostream&
  operator<< (std::ostream& o, const Problem<F, C>& pb)
  {
    return pb.print (o);
  }
  /**
     @}
  */
}; // end of namespace roboptim
# include <roboptim-core/problem.hxx>
#endif //! ROBOPTIM_CORE_PROBLEM_HH
