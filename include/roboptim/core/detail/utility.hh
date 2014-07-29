// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_DETAIL_UTILITY_HH
# define ROBOPTIM_CORE_DETAIL_UTILITY_HH
# include <boost/mpl/assert.hpp>
# include <boost/mpl/count_if.hpp>
# include <boost/mpl/greater.hpp>
# include <boost/mpl/logical.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/type_traits/is_base_of.hpp>

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
    ///
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


    /// \brief Whether a sequence of types contains a base of a given type.
    /// \tparam Sequence sequence of types.
    /// \tparam Type type.
    template <typename Sequence, typename Type>
    struct contains_base_of
      : greater<int_<count_if<Sequence, is_base_of<_, Type> >::value>, int_<0> >
    {};


    /// \brief Get the descendant among two relatives.
    /// Type1 and Type2 are expected to be base/derivative of one another.
    /// \tparam Type1 first relative.
    /// \tparam Type2 second relative.
    template <typename Type1, typename Type2>
    struct get_descendant
      : if_<is_base_of<Type1, Type2>, Type2, Type1>
    {
      BOOST_MPL_ASSERT_MSG((or_<is_base_of<Type1, Type2>,
			    is_base_of<Type2, Type1> >::value),
                           ONE_SHOULD_INHERIT_FROM_THE_OTHER, (Type1, Type2));
    };


    /// \brief Checks whether C is a valid constraint type in CLIST.
    /// \tparam C constraint type.
    /// \tparam CLIST a vector of constraint types.
    template <typename C, typename CLIST>
    struct check_constraint_type
      : fold<CLIST,
	     bool_<false>,
	     if_<is_base_of<_2, C>, bool_<true>, _1> >
    {};


    /// \brief Get the constraint type of CLIST that best match C.
    /// \tparam C constraint type.
    /// \tparam CLIST a vector of constraint types.
    template <typename C, typename CLIST>
    struct cast_constraint_type
    {
      typedef typename
      fold <CLIST,
            void,
            if_<is_base_of<_2, C>,
                mpl::if_<is_void<_1>, _2, detail::get_descendant<_1, _2> >,
                _1>
	    >::type C_type;
    };


    /// \brief Convert a constraint to a proper type.
    /// \tparam CLIST a vector of valid constraint types.
    template <typename CLIST>
    struct ConvertConstraint
    {
      template <typename C>
      boost::shared_ptr<typename cast_constraint_type<C, CLIST>::C_type>
      operator () (const boost::shared_ptr<C>& c) const
      {
        return boost::static_pointer_cast
          <typename cast_constraint_type<C, CLIST>::C_type> (c);
      }
    };


    /// \brief Convert a constraint from a Boost.Variant to
    /// an adequate constraint type depending on the problem's
    /// constraints type.
    ///
    /// \tparam P problem type.
    template <typename P>
    struct ConvertConstraintVariant
      : public boost::static_visitor<typename P::constraint_t>
    {
      template <typename U>
      typename P::constraint_t operator () (const U& c) const
      {
        ConvertConstraint<typename P::constraintsList_t> converter;
        return converter (c);
      }
    };
  } // end of namespace detail.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_DETAIL_UTILITY_HH
