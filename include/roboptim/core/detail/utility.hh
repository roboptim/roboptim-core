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
# include <boost/mpl/back_inserter.hpp>
# include <boost/mpl/copy.hpp>
# include <boost/mpl/count_if.hpp>
# include <boost/mpl/fold.hpp>
# include <boost/mpl/greater.hpp>
# include <boost/mpl/has_xxx.hpp>
# include <boost/mpl/logical.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/variant.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <vector>

# include <Eigen/Core>
# include <Eigen/StdVector>

namespace roboptim
{
  namespace detail
  {
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


    /// \brief Generate a Boost.Variant of shared pointers from the static
    /// constraints types list.
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
    /// \tparam CLIST vector of types
    template <typename CLIST>
    struct shared_ptr_variant :
      boost::make_variant_over<typename detail::add_shared_ptr<CLIST>::type>
    {};

    BOOST_MPL_HAS_XXX_TRAIT_DEF (Scalar)
    BOOST_MPL_HAS_XXX_TRAIT_DEF (Index)
    BOOST_MPL_HAS_XXX_TRAIT_DEF (StorageKind)

    /// \brief Check whether the type provided is an Eigen type.
    ///
    /// This solution comes from a post in the Eigen forums:
    /// https://forum.kde.org/viewtopic.php?f=74&t=121280
    ///
    /// \tparam T
    template<typename T>
    struct is_eigen_type
      : boost::mpl::and_<has_Scalar<T>,
                         has_Index<T>,
                         has_StorageKind<T> >
    {};


    /// \brief Return an Eigen-aligned std::vector if required, else use the
    /// default allocator.
    /// \tparam V type of the elements.
    template <typename V>
    struct aligned_vector_type
      : boost::mpl::if_<
          is_eigen_type<V>,
          typename boost::mpl::if_c<(sizeof (V) % 16) == 0,
                                    std::vector<V, Eigen::aligned_allocator<V> >,
                                    std::vector<V> >::type,
          std::vector<V> >
    {};


    /// \brief Return the type of a const reference to an Eigen matrix, using
    /// Eigen's Ref.
    ///
    /// \tparam T Eigen dense matrix/vector type.
    template <typename T>
    struct const_eigen_ref
    {
      typedef const Eigen::Ref<const T>& type;
    };

    /// \brief Return the proper const reference type of a given type.
    ///
    /// For instance:
    ///   * const_ref<float>::value_t == const float&
    ///   * const_ref<argument_t>::value_t == const_argument_ref
    ///
    /// This returns a const Eigen::Ref for dense Eigen matrices, else a
    /// simple const reference.
    ///
    /// Note: this currently does not cover all cases (e.g. sparse
    /// vectors), but should work for argument_t/vector_t (dense
    /// vectors).
    ///
    /// \tparam T type.
    template <typename T>
    struct const_ref
    {
    typedef typename
      boost::mpl::if_<is_eigen_type<T>,
                      const_eigen_ref<T>,
                      boost::add_reference<typename boost::add_const<T>::type> >
      ::type::type type;
    };


    /// \brief Get the matrix stride type for a row vector, given a matrix
    /// storage order.
    ///
    /// This solves compiler errors in Eigen when dealing with Eigen::Refs,
    /// row vectors and different storage orders.
    ///
    /// \tparam SO storage order (Eigen::ColMajor or Eigen::RowMajor).
    template <int SO>
    struct row_vector_stride
    {
      typedef Eigen::InnerStride<(SO == Eigen::RowMajor)? 1:-1> type;
    };


    /// \brief Converts CLIST to a boost::mpl::vector to ensure a similar
    /// behavior for codes using different random access sequences (vector,
    /// list, etc.).
    ///
    /// In the case of boost::mpl::vector, this ensures a normalized
    /// representation of the vector (boost::mpl::vector converted to
    /// boost::mpl::v_item) and orders the constraints in a proper way. This
    /// makes the use of typeid comparison possible.
    ///
    /// \tparam CLIST vector of types
    template <typename CLIST>
    struct list_converter :
      boost::mpl::copy
        <CLIST, boost::mpl::back_inserter<boost::mpl::vector<> > >
    {};


    /// \brief Whether a sequence of types contains a base of a given type.
    /// \tparam Sequence sequence of types.
    /// \tparam Type type.
    template <typename Sequence, typename Type>
    struct contains_base_of
      : boost::mpl::greater<
          boost::mpl::int_<
            boost::mpl::count_if<Sequence,
                                 boost::is_base_of<boost::mpl::_, Type> >
            ::value>,
          boost::mpl::int_<0> >
    {};


    /// \brief Get the descendant among two relatives.
    /// Type1 and Type2 are expected to be base/derivative of one another.
    /// \tparam Type1 first relative.
    /// \tparam Type2 second relative.
    template <typename Type1, typename Type2>
    struct get_descendant
      : boost::mpl::if_<boost::is_base_of<Type1, Type2>, Type2, Type1>
    {
      BOOST_MPL_ASSERT_MSG((boost::mpl::or_<boost::is_base_of<Type1, Type2>,
			    boost::is_base_of<Type2, Type1> >::value),
                           ONE_SHOULD_INHERIT_FROM_THE_OTHER, (Type1&, Type2&));
    };


    /// \brief Checks whether C is a valid constraint type in CLIST.
    /// \tparam C constraint type.
    /// \tparam CLIST a vector of constraint types.
    template <typename C, typename CLIST>
    struct check_constraint_type
      : boost::mpl::fold<CLIST,
          boost::mpl::bool_<false>,
          boost::mpl::if_<boost::is_base_of<boost::mpl::_2, C>,
                          boost::mpl::bool_<true>,
                          boost::mpl::_1>
        >
    {};


    /// \brief Get the constraint type of CLIST that best match C.
    /// \tparam C constraint type.
    /// \tparam CLIST a vector of constraint types.
    template <typename C, typename CLIST>
    struct cast_constraint_type
    {
      typedef typename
      boost::mpl::fold <CLIST,
            void,
            boost::mpl::if_<boost::is_base_of<boost::mpl::_2, C>,
                boost::mpl::if_<boost::is_void<boost::mpl::_1>,
                         boost::mpl::_2,
                         detail::get_descendant<boost::mpl::_1, boost::mpl::_2> >,
                boost::mpl::_1>
	    >::type type;
    };


    /// \brief Checks whether the function types derives from Function or
    /// SparseFunction.
    ///
    /// \tparam F function type.
    template <typename F>
    struct derives_from_function :
      boost::mpl::or_<boost::is_base_of<Function, F>,
                      boost::is_base_of<SparseFunction, F> >
    {};

    /// \brief Checks whether the function type derives from
    /// DifferentiableFunction or DifferentiableSparseFunction.
    ///
    /// \tparam F function type.
    template <typename F>
    struct derives_from_differentiable_function :
      boost::mpl::or_<boost::is_base_of<DifferentiableFunction, F>,
                      boost::is_base_of<DifferentiableSparseFunction, F> >
    {};

    /// \brief Checks whether the function type derives from
    /// TwiceDifferentiableFunction or TwiceDifferentiableSparseFunction.
    ///
    /// \tparam F function type.
    template <typename F>
    struct derives_from_twice_differentiable_function :
      boost::mpl::or_<boost::is_base_of<TwiceDifferentiableFunction, F>,
                      boost::is_base_of<TwiceDifferentiableSparseFunction, F> >
    {};

    /// \brief Checks whether the function type derives from
    /// NTimesDerivableFunction.
    ///
    /// \tparam F function type.
    template <typename F>
    struct derives_from_ntimes_derivable_function :
      boost::is_base_of<NTimesDerivableFunction<2>, F>
    {};

    /// \brief Checks whether all the constraints derive from Function or
    /// SparseFunction.
    ///
    /// \tparam CLIST a vector of constraint types.
    template <typename CLIST>
    struct list_derives_from_function :
      boost::mpl::fold<CLIST,
                       boost::mpl::bool_<true>,
                       boost::mpl::if_<
                         derives_from_function<boost::mpl::_2>,
                         boost::mpl::_1,
                         boost::mpl::bool_<false> >
      >
    {};

    /// \brief Check that CLIST_ is a subset of CLIST (i.e. all the
    /// functions of CLIST_ derive from functions of CLIST).
    /// \tparam CLIST_ a vector of constraint types.
    /// \tparam CLIST a vector of constraint types.
    template <typename CLIST_, typename CLIST>
    struct is_compatible_list
    {
      // The algorithm is as follows:
      // (0) outer_ok = true
      // (1) for c_ in FLIST_:
      // (2)   inner_ok = false
      // (3)   for c in FLIST:
      // (4)     if c is_base_of c_:
      // (5)       inner_ok = true
      // (6)   if not inner_ok:
      // (7)     outer_ok = false
      // (8) return outer_ok
      typedef typename
      boost::mpl::fold<CLIST_, // (1)
                       boost::mpl::bool_<true>, // (0)
                       boost::mpl::if_< // (6)
                         contains_base_of<CLIST,boost::mpl::_2>, // (2,3,4,5)
                         boost::mpl::_1,
                         boost::mpl::bool_<false> // (7)
                       >
       >::type type;
    };


    /// \brief Convert a constraint to a proper type.
    /// \tparam CLIST a vector of valid constraint types.
    template <typename CLIST>
    struct ConvertConstraint
    {
      template <typename C>
      boost::shared_ptr<typename cast_constraint_type<C, CLIST>::type>
      operator () (const boost::shared_ptr<C>& c) const
      {
        return boost::static_pointer_cast
          <typename cast_constraint_type<C, CLIST>::type> (c);
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
