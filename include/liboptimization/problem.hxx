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
# include <boost/type_traits/remove_pointer.hpp>

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
    BOOST_STATIC_ASSERT((boost::is_convertible<F_*, F*>::value));
    // Check that C is a subtype of C_.

    typedef typename boost::remove_pointer<C_>::type rpC_;
    typedef typename boost::remove_pointer<C>::type rpC;
    BOOST_STATIC_ASSERT((boost::is_convertible<rpC_*, rpC*>::value));

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

}; // end of namespace optimization

# include <liboptimization/problem.hxx>
#endif //! OPTIMIZATION_PROBLEM_HH
