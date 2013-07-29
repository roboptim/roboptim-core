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

#ifndef ROBOPTIM_CORE_FILTER_PLUS_HH
# define ROBOPTIM_CORE_FILTER_PLUS_HH
# include <vector>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/differentiable-function.hh>


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
      typedef T T_numtype;
    };

# define ROBOPTIM_CORE_DECLARE_AUTOPROMOTE(T1,T2)		\
    template<>							\
    struct AutopromoteTrait<T1>					\
    {								\
      typedef T2 T_numtype;					\
    }

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
      typedef typename AutopromoteTrait<T1_orig>::T_numtype T1;
      typedef typename AutopromoteTrait<T2_orig>::T_numtype T2;

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


namespace roboptim
{
  /// \brief Sum two RobOptim functions.
  template <typename U, typename V>
  class Plus : public detail::PromoteTrait<U, V>::T_promote
  {
  public:
    typedef typename detail::PromoteTrait<U, V>::T_promote parentType_t;
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (parentType_t);

    typedef boost::shared_ptr<Plus> PlusShPtr_t;

    explicit Plus (boost::shared_ptr<U> left, boost::shared_ptr<V> right) throw ();
    ~Plus () throw ();

    const boost::shared_ptr<U>& left () const
    {
      return left_;
    }

    U& left ()
    {
      return left_;
    }

    const boost::shared_ptr<V>& right () const
    {
      return right_;
    }

    V& right ()
    {
      return right_;
    }

    void impl_compute (result_t& result, const argument_t& x)
      const throw ();

    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			size_type functionId = 0)
      const throw ();
    void impl_jacobian (jacobian_t& jacobian,
			const argument_t& arg)
      const throw ();
  private:
    boost::shared_ptr<U> left_;
    boost::shared_ptr<V> right_;

    mutable result_t result_;
    mutable gradient_t gradient_;
    mutable jacobian_t jacobian_;
  };

  template <typename U, typename V>
  boost::shared_ptr<Plus<U, V> >
  plus (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
  {
    return boost::make_shared<Plus<U, V> > (left, right);
  }

} // end of namespace roboptim.

# include <roboptim/core/filter/plus.hxx>
#endif //! ROBOPTIM_CORE_FILTER_PLUS_HH
