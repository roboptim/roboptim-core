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

#ifndef ROBOPTIM_CORE_FILTER_PRODUCT_HXX
# define ROBOPTIM_CORE_FILTER_PRODUCT_HXX
# include <boost/format.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_same.hpp>
# include <boost/mpl/and.hpp>

namespace roboptim
{
  namespace detail
  {
    /// \brief Utility structure used for product differentiation.
    struct ProductDifferentiation
    {
      /// \brief Some useful types for product differentiation.
      /// \tparam U left function.
      /// \tparam V right function.
      template <typename U, typename V>
      struct Types
      {
        typedef typename boost::mpl::and_<
          boost::is_same<typename U::traits_t, EigenMatrixDense>,
          boost::is_same<typename V::traits_t, EigenMatrixDense> >
            fullDense_t;

        typedef typename Product<U,V>::size_type  size_type;
        typedef typename Product<U,V>::value_type value_type;

        typedef typename U::vector_t vectorU_t;
        typedef typename V::vector_t vectorV_t;
        typedef typename U::vector_ref vectorU_ref;
        typedef typename V::vector_ref vectorV_ref;

        typedef typename U::gradient_t gradientU_t;
        typedef typename V::gradient_t gradientV_t;
        typedef typename Product<U,V>::gradient_t gradient_t;
        typedef typename U::gradient_ref gradientU_ref;
        typedef typename V::gradient_ref gradientV_ref;
        typedef typename Product<U,V>::gradient_ref gradient_ref;

        typedef typename U::jacobian_t jacobianU_t;
        typedef typename V::jacobian_t jacobianV_t;
        typedef typename Product<U,V>::jacobian_t jacobian_t;
        typedef typename U::jacobian_ref jacobianU_ref;
        typedef typename V::jacobian_ref jacobianV_ref;
        typedef typename Product<U,V>::jacobian_ref jacobian_ref;
      };


      /// \brief Full dense version of gradient computation.
      template <typename U, typename V>
      static void gradient
      (typename Types<U,V>::gradient_ref grad_uv,
       const typename Types<U,V>::vectorU_ref u,
       const typename Types<U,V>::vectorV_ref v,
       const typename Types<U,V>::gradientU_ref grad_u,
       const typename Types<U,V>::gradientV_ref grad_v,
       typename boost::enable_if<typename Types<U,V>::fullDense_t>::type* = 0)
      {
        grad_uv.noalias () = u.cwiseProduct (grad_v);
        grad_uv.noalias () += v.cwiseProduct (grad_u);
      }

      /// \brief Dense/sparse version of gradient computation.
      /// dense.cwiseProduct (sparse) or sparse.cwiseProduct (dense) are
      /// apparently not available in Eigen.
      template <typename U, typename V>
      static void gradient
      (typename Types<U,V>::gradient_ref grad_uv,
       const typename Types<U,V>::vectorU_ref u,
       const typename Types<U,V>::vectorV_ref v,
       const typename Types<U,V>::gradientU_ref grad_u,
       const typename Types<U,V>::gradientV_ref grad_v,
       typename boost::disable_if<typename Types<U,V>::fullDense_t>::type* = 0)
      {
        // Here, u and v are dense vectors, while grad_u and grad_v are sparse
        // vectors. Since cwiseProduct is not available for dense-sparse
        // computation, we compute it manually. All we need to do is loop over
        // the nonzeros of grad_u or grad_v and multiply by v or u.
        grad_uv.setZero ();

        // grad_uv = u * grad_v;
        for (typename Types<U,V>::gradientV_t::InnerIterator it (grad_v);
             it; ++it)
	  {
	    typename Types<U,V>::gradientV_t::Index id = it.index ();
	    grad_uv.coeffRef (id) = u (id) * it.value ();
	  }

        // grad_uv += v * grad_u;
        for (typename Types<U,V>::gradientU_t::InnerIterator it (grad_u);
             it; ++it)
	  {
	    typename Types<U,V>::gradientU_t::Index id = it.index ();
	    grad_uv.coeffRef (id) += v (id) * it.value ();
	  }
      }

      /// \brief Full dense version of Jacobian computation.
      template <typename U, typename V>
      static void jacobian
      (typename Types<U,V>::jacobian_ref jac_uv,
       const typename Types<U,V>::vectorU_ref u,
       const typename Types<U,V>::vectorV_ref v,
       const typename Types<U,V>::jacobianU_ref jac_u,
       const typename Types<U,V>::jacobianV_ref jac_v,
       typename boost::enable_if<typename Types<U,V>::fullDense_t>::type* = 0)
      {
        // For each column of the Jacobian
        // FIXME: do it Eigen-style
        for (typename Types<U,V>::jacobian_t::Index i = 0;
             i < jac_uv.cols (); ++i)
	  {
	    jac_uv.col (i).noalias () = u.cwiseProduct (jac_v.col (i));
	    jac_uv.col (i).noalias () += v.cwiseProduct (jac_u.col (i));
	  }
      }

      /// \brief Dense/sparse version of Jacobian computation.
      /// dense.cwiseProduct (sparse) or sparse.cwiseProduct (dense) are
      /// apparently not available in Eigen.
      template <typename U, typename V>
      static void jacobian
      (typename Types<U,V>::jacobian_ref jac_uv,
       const typename Types<U,V>::vectorU_ref u,
       const typename Types<U,V>::vectorV_ref v,
       const typename Types<U,V>::jacobianU_ref jac_u,
       const typename Types<U,V>::jacobianV_ref jac_v,
       typename boost::disable_if<typename Types<U,V>::fullDense_t>::type* = 0)
      {
        jac_uv.setZero ();

        typedef Eigen::Triplet<typename Types<U,V>::value_type> triplet_t;
        std::vector<triplet_t> tripletList;
        tripletList.reserve
          (static_cast<std::size_t>
           (std::max<typename Types<U,V>::size_type>
            (jac_u.nonZeros (), jac_v.nonZeros ())));

        // tripletList will contain duplicated elements that will be summed up
        // by setFromTriplets()

        // For each column of the Jacobian
        // FIXME: do it Eigen-style
        for (typename Types<U,V>::jacobian_t::Index i = 0;
             i < jac_uv.cols (); ++i)
	  {
	    // grad_uv = u * grad_v;
	    for (typename Types<U,V>::jacobianV_t::InnerIterator
		   it (jac_v, i); it; ++it)
	      {
		typename Types<U,V>::gradientV_t::Index id = it.index ();
		tripletList.push_back (triplet_t (id, i, u (id) * it.value ()));
	      }

	    // grad_uv += v * grad_u;
	    for (typename Types<U,V>::jacobianU_t::InnerIterator
		   it (jac_u, i); it; ++it)
	      {
		typename Types<U,V>::gradientU_t::Index id = it.index ();
		tripletList.push_back (triplet_t (id, i, v (id) * it.value ()));
	      }
	  }

        jac_uv.setFromTriplets (tripletList.begin (), tripletList.end ());
      }
    };
  } // end of namespace detail.

  template <typename U, typename V>
  Product<U, V>::Product
  (boost::shared_ptr<U> left, boost::shared_ptr<V> right)
    : detail::PromoteTrait<U, V>::T_promote
      (left->inputSize (),
       left->outputSize (),
       (boost::format ("(%1%) * (%2%)")
	% left->getName ()
	% right->getName ()).str ()),
      left_ (left),
      right_ (right),
      resultLeft_ (left->outputSize ()),
      resultRight_ (left->outputSize ()),
      gradientLeft_ (left->inputSize ()),
      gradientRight_ (left->inputSize ()),
      jacobianLeft_ (left->outputSize (),
		     left->inputSize ()),
      jacobianRight_ (left->outputSize (),
		      left->inputSize ())
  {
    if (left->inputSize () != right->inputSize ()
	|| left->outputSize () != right->outputSize ())
      throw std::runtime_error ("left and right size mismatch");
    resultLeft_.setZero ();
    resultRight_.setZero ();
    gradientLeft_.setZero ();
    gradientRight_.setZero ();
    jacobianLeft_.setZero ();
    jacobianRight_.setZero ();
  }

  template <typename U, typename V>
  Product<U, V>::~Product ()
  {}

  template <typename U, typename V>
  void
  Product<U, V>::impl_compute
  (result_ref result, const_argument_ref x)
    const
  {
    (*left_) (resultLeft_, x);
    (*right_) (resultRight_, x);
    result.noalias () = resultLeft_.cwiseProduct (resultRight_);
  }

  template <typename U, typename V>
  void
  Product<U, V>::impl_gradient (gradient_ref gradient,
				const_argument_ref x,
				size_type functionId)
    const
  {
    // Compute grad_U and grad_V
    left_->gradient (gradientLeft_, x, functionId);
    right_->gradient (gradientRight_, x, functionId);

    // Compute U and V
    // FIXME: this is highly inefficient (computed once for each gradient)
    (*left_) (resultLeft_, x);
    (*right_) (resultRight_, x);

    // Compute gradient = ∂U V + ∂V U
    detail::ProductDifferentiation::gradient<U,V>
      (gradient, resultLeft_, resultRight_,
       gradientLeft_, gradientRight_);
  }


  template <typename U, typename V>
  void
  Product<U, V>::impl_jacobian (jacobian_ref jacobian,
				const_argument_ref x)
    const
  {
    // Compute U and V
    (*left_) (resultLeft_, x);
    (*right_) (resultRight_, x);

    // Compute Jac(U) and Jac(V)
    left_->jacobian (jacobianLeft_, x);
    right_->jacobian (jacobianRight_, x);

    // Compute the Jacobian
    detail::ProductDifferentiation::jacobian<U,V>
      (jacobian, resultLeft_, resultRight_,
       jacobianLeft_, jacobianRight_);
  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_PRODUCT_HXX
