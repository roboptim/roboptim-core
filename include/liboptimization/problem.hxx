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
 * \brief Implementation of the Problem class.
 */

#ifndef OPTIMIZATION_PROBLEM_HXX
# define OPTIMIZATION_PROBLEM_HXX
# include <algorithm>
# include <boost/numeric/ublas/io.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/variant.hpp>
# include <boost/variant/get.hpp>

namespace optimization
{
  template <typename F, typename C>
  Problem<F, C>::Problem (const function_t& f) throw ()
    : function_ (f),
      startingPoint_ (),
      constraints_ ()
  {
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
      constraints_ (pb.constraints_)
  {
  }

  // Copy constructor (convert from another class of problem).
  template <typename F, typename C>
  template <typename F_, typename C_>
  Problem<F, C>::Problem (const Problem<F_, C_>& pb) throw ()
    : function_ (pb.function_),
      startingPoint_ (pb.startingPoint_),
      constraints_ ()
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
  typename Problem<F, C>::constraints_t&
  Problem<F, C>::constraints () throw ()
  {
    return constraints_;
  }

  template <typename F, typename C>
  const typename Problem<F, C>::constraints_t&
  Problem<F, C>::constraints () const throw ()
  {
    return constraints_;
  }

  template <typename F, typename C>
  void
  Problem<F, C>::addConstraint (const C& x) throw ()
  {
    constraints_.push_back (x);
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


  namespace detail
  {
    template <typename T1, typename T2>
    Function::value_type
    impl_get_infinity (const boost::variant<T1, T2>& variant)
    {
      switch (variant.which ())
        {
        case 0:
          return boost::get<T1> (variant)->infinity;
        case 1:
          return boost::get<T2> (variant)->infinity;
        default:
          assert (0);
        }
    }

    template <typename T>
    Function::value_type
    impl_get_infinity (const T* t)
    {
      return t->infinity;
    }

    template <typename T>
    Function::value_type
    impl_get_infinity (const T& t)
    {
      return t.infinity;
    }
  }

  template <typename F, typename C>
  typename Problem<F, C>::function_t::value_type
  Problem<F, C>::infinity () const throw ()
  {
    Function::value_type res = function_.infinity;
    typedef typename constraints_t::const_iterator citer_t;
    for (citer_t it = constraints_.begin ();
         it != constraints_.end (); ++it)
      res = std::min (res, detail::impl_get_infinity (*it));
    return res;
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
    o << function () << std::endl;
    if (constraints ().empty ())
      o << "No constraints";
    else
      o << "Number of constraints: " << constraints ().size ();

    typedef typename constraints_t::const_iterator citer_t;

    if (boost::is_pointer<C> ())
      for (citer_t it = constraints ().begin ();
           it != constraints ().end ();
           ++it)
        {
          o << std::endl;
          detail::impl_print (o, *it);
        }
    if (startingPoint_)
      o << std::endl << "Starting point: " << *startingPoint_;
    else
      o << std::endl << "No starting point.";
    return o;
  }

  template <typename F, typename C>
  std::ostream&
  operator<< (std::ostream& o, const Problem<F, C>& pb)
  {
    return pb.print (o);
  }
}; // end of namespace optimization

# include <liboptimization/problem.hxx>
#endif //! OPTIMIZATION_PROBLEM_HH
