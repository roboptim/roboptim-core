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
 * \brief Implementation of the problem_converter function.
 */

#ifndef OPTIMIZATION_PROBLEM_CONVERTER_HXX
# define OPTIMIZATION_PROBLEM_CONVERTER_HXX
# include <boost/mpl/and.hpp>

namespace optimization
{
  namespace detail
  {
    // The source problem is a subproblem of the destination one.
    // Problem is copied directly.
    template <typename F, typename C, typename F_, typename C_, typename T>
    Problem<F, C>
    impl_problem_converter (const Problem<F_, C_>& src, const boost::true_type&, T)
    {
      return Pb (src);
    }

    // The problem is not a subproblem, try to dynamic_cast (with pointer).
    template <typename F, typename C, typename F_, typename C_>
    Problem<F, C>
    impl_problem_converter (const Problem<F_, C_>& src, const boost::false_type&,
                            const boost::true_type&)
    {
      Problem<F, C> pb (src.function ());

      // Copy constraints through a dynamic_cast.
      typedef typename Problem<F_, C_>::constraints_t::const_iterator citer_t;
      for (citer_t it = src.constraints ().begin ();
           it != src.constraints ().end (); ++it)
        {
          C ptr = dynamic_cast<C> (*it);
          if (!ptr)
            throw std::bad_cast ();
          pb.constraints ().push_back (ptr);
        }

      return pb;
    }

    // The problem is not a subproblem, try to dynamic_cast (with reference).
    template <typename F, typename C, typename F_, typename C_>
    Problem<F, C>
    impl_problem_converter (const Problem<F_, C_>& src, const boost::false_type&,
                            const boost::false_type&)
    {
      Problem<F, C> pb (src.function ());

      // Copy constraints through a dynamic_cast.
      typedef typename Problem<F_, C_>::constraints_t::const_iterator citer_t;
      for (citer_t it = src.constraints ().begin ();
           it != src.constraints ().end (); ++it)
        pb.constraints ().push_back (dynamic_cast<C> (*it));
      return pb;
    }
  }; // end of namespace detail.


  // If same problem, copy it.
  template <typename F, typename C>
  Problem<F, C>
  problem_converter (const Problem<F, C>& src) throw (std::bad_cast)
  {
    return Problem<F, C> (src);
  }

  // If two different classes of problem, try to convert from
  // one to another.
  template <typename F, typename C, typename F_, typename C_>
  Problem<F, C>
  problem_converter (const Problem<F_, C_>& src) throw (std::bad_cast)
  {
    typedef boost::mpl::and_
      <boost::is_convertible<F*, F_*>, boost::is_convertible<C*, C_*> >
      is_subproblem_t;

    typedef boost::is_pointer<C> is_pointer_t;

    return impl_problem_converter (src,
                                   is_subproblem_t (),
                                   is_pointer_t ());
  }

  // If not a problem, fail at compile-time.
  template <typename PBSRC, typename PBDST>
  PBDST
  problem_converter (const PBSRC& src) throw (std::bad_cast)
  {
    BOOST_STATIC_ASSERT (sizeof (PBSRC) == -42);
  }

}; // end of namespace optimization

#endif //! OPTIMIZATION_PROBLEM_CONVERTER_HXX
