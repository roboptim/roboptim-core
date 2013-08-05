// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_DETAIL_AUTOPROMOTE_HH
# define ROBOPTIM_CORE_DETAIL_AUTOPROMOTE_HH
# include <boost/mpl/assert.hpp>
# include <roboptim/core/fwd.hh>

namespace roboptim
{
  namespace detail
  {
    template<class T>
    struct PrecisionTrait
    {
      enum
	{
	  precisionRank = 0,
	  knowPrecisionRank = 0
	};
    };

# define ROBOPTIM_CORE_DECLARE_PRECISION(T,rank)	\
    template<>						\
    struct PrecisionTrait< T >				\
    {							\
      enum {						\
	precisionRank = rank,				\
	knowPrecisionRank = 1				\
      };						\
    }

    ROBOPTIM_CORE_DECLARE_PRECISION (GenericFunction<EigenMatrixDense>, 1);
    ROBOPTIM_CORE_DECLARE_PRECISION (GenericFunction<EigenMatrixSparse>, 1);

    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericDifferentiableFunction<EigenMatrixDense>, 2);
    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericDifferentiableFunction<EigenMatrixSparse>, 2);

    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericTwiceDifferentiableFunction<EigenMatrixDense>, 3);
    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericTwiceDifferentiableFunction<EigenMatrixSparse>, 3);

    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericQuadraticFunction<EigenMatrixDense>, 3);
    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericQuadraticFunction<EigenMatrixSparse>, 3);

    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericLinearFunction<EigenMatrixDense>, 4);
    ROBOPTIM_CORE_DECLARE_PRECISION
    (GenericLinearFunction<EigenMatrixSparse>, 4);



    template<class T>
    struct AutopromoteTrait
    {
      typedef T T_type;
    };

# define ROBOPTIM_CORE_DECLARE_AUTOPROMOTE(T1,T2)		\
    template<>							\
    struct AutopromoteTrait<T1>					\
    {								\
      typedef T2 T_type;					\
    }

    template <typename U, typename V>
    struct AutopromoteTrait<Plus<U, V> >
    {
      typedef typename Plus<U, V>::parent_t T_type;
    };

    template <typename U, typename V>
    struct AutopromoteTrait<Minus<U, V> >
    {
      typedef typename Minus<U, V>::parent_t T_type;
    };

    template <typename U, typename V>
    struct AutopromoteTrait<Product<U, V> >
    {
      typedef typename Product<U, V>::parent_t T_type;
    };

    template <typename U>
    struct AutopromoteTrait<Scalar<U> >
    {
      typedef typename Scalar<U>::parent_t T_type;
    };

    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericNumericQuadraticFunction<EigenMatrixDense>,
     GenericQuadraticFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericNumericQuadraticFunction<EigenMatrixSparse>,
     GenericQuadraticFunction<EigenMatrixSparse>);

    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericNumericLinearFunction<EigenMatrixDense>,
     GenericLinearFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericNumericLinearFunction<EigenMatrixSparse>,
     GenericLinearFunction<EigenMatrixSparse>);


    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericConstantFunction<EigenMatrixDense>,
     GenericLinearFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericConstantFunction<EigenMatrixSparse>,
     GenericLinearFunction<EigenMatrixSparse>);

    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericIdentityFunction<EigenMatrixDense>,
     GenericLinearFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (GenericIdentityFunction<EigenMatrixSparse>,
     GenericLinearFunction<EigenMatrixSparse>);

    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (Cos<EigenMatrixDense>,
     GenericDifferentiableFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (Cos<EigenMatrixSparse>,
     GenericDifferentiableFunction<EigenMatrixSparse>);

    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (Sin<EigenMatrixDense>,
     GenericDifferentiableFunction<EigenMatrixDense>);
    ROBOPTIM_CORE_DECLARE_AUTOPROMOTE
    (Sin<EigenMatrixSparse>,
     GenericDifferentiableFunction<EigenMatrixSparse>);



    template<class T1, class T2, int promoteToT1>
    struct promote2
    {
      typedef T1 T_promote;
    };

    template<class T1, class T2>
    struct promote2<T1,T2,0>
    {
      typedef T2 T_promote;
    };

    template<class T1_orig, class T2_orig>
    struct PromoteTrait
    {
      // Handle auto promotion.
      typedef typename AutopromoteTrait<T1_orig>::T_type T1;
      typedef typename AutopromoteTrait<T2_orig>::T_type T2;

      enum
	{
	  // True if T1 is higher ranked
	  T1IsBetter =
	  PrecisionTrait<T1>::precisionRank >
	  PrecisionTrait<T2>::precisionRank,
	  // True if we know ranks for both T1 and T2
	  knowBothRanks =
	  PrecisionTrait<T1>::knowPrecisionRank
	  && PrecisionTrait<T2>::knowPrecisionRank
	};

      // If we don't know both ranks, assert.
      BOOST_MPL_ASSERT ((boost::mpl::bool_<knowBothRanks>));

      // Promote to type having the lesser precision.
      enum
	{
	  promoteToT1 = (T1IsBetter) ? false : true
	};

      typedef typename promote2<T1, T2, promoteToT1>::T_promote T_promote;
    };
  } // end of namespace detail.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_DETAIL_AUTOPROMOTE_HH
