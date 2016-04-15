// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_DECORATOR_CACHED_FUNCTION_HXX
# define ROBOPTIM_CORE_DECORATOR_CACHED_FUNCTION_HXX

# include <boost/format.hpp>
# include <boost/utility/enable_if.hpp>

# include <roboptim/core/derivative-size.hh>
# include <roboptim/core/n-times-derivable-function.hh>
# include <roboptim/core/detail/utility.hh>

namespace roboptim
{
  namespace
  {
    template <typename T>
    std::string cachedFunctionName (const T& fct);

    template <typename T>
    std::string cachedFunctionName (const T& fct)
    {
      boost::format fmt ("%1% (cached)");
      fmt % fct.getName ();
      return fmt.str ();
    }
  } // end of anonymous namespace.

  namespace detail
  {
    template <typename T>
    struct CachedFunctionTypes
    {
      typedef typename T::traits_t traits_t;
      typedef CachedFunction<T> cachedFunction_t;

      ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
      (GenericTwiceDifferentiableFunction<traits_t>);

      typedef typename boost::enable_if<detail::
					derives_from_differentiable_function<T> >
      isDifferentiable_t;

      typedef typename boost::disable_if<detail::
					 derives_from_differentiable_function<T> >
      isNotDifferentiable_t;

      typedef typename boost::enable_if<detail::
					derives_from_twice_differentiable_function<T> >
      isTwiceDifferentiable_t;

      typedef typename boost::disable_if<detail::
					 derives_from_twice_differentiable_function<T> >
      isNotTwiceDifferentiable_t;

      typedef typename boost::enable_if<detail::
					derives_from_ntimes_derivable_function<T> >
      isNTimesDerivable_t;

      typedef typename boost::disable_if<detail::
					 derives_from_ntimes_derivable_function<T> >
      isNotNTimesDerivable_t;
    };
  } // end of namespace detail

  template <typename T>
  CachedFunction<T>::CachedFunction (boost::shared_ptr<T> fct,
                                     size_t size)
    : T (fct->inputSize (), fct->outputSize (), cachedFunctionName (*fct)),
      function_ (fct),
      cache_ (derivativeSize<T>::value, size),
      gradientCache_ (static_cast<std::size_t> (fct->outputSize ()), size),
      hessianCache_ (static_cast<std::size_t> (fct->outputSize ()), size)
  {
  }

  template <typename T>
  CachedFunction<T>::~CachedFunction ()
  {
  }

  template <typename T>
  void
  CachedFunction<T>::reset ()
  {
    for (typename std::vector<functionCache_t>::iterator
	   iter  = cache_.begin ();
         iter != cache_.end ();
         ++iter)
      {
	iter->clear ();
      }

    for (typename std::vector<gradientCache_t>::iterator
	   iter  = gradientCache_.begin ();
         iter != gradientCache_.end ();
         ++iter)
      {
	iter->clear ();
      }

    jacobianCache_.clear ();

    for (typename std::vector<hessianCache_t>::iterator
	   iter  = hessianCache_.begin ();
         iter != hessianCache_.end ();
         ++iter)
      {
	iter->clear ();
      }
  }

  template <typename T>
  std::ostream&
  CachedFunction<T>::print (std::ostream& o) const
  {
    o << this->getName () << ":" << incindent
      << iendl << *function_
      << iendl << "Cache size: " << cache_[0].size ()
      << decindent;
    return o;
  }

  template <typename T>
  const boost::shared_ptr<const T>
  CachedFunction<T>::function () const
  {
    return function_;
  }

  template <typename T>
  void
  CachedFunction<T>::impl_compute (result_ref result,
				   const_argument_ref argument)
    const
  {
    typename CachedFunction<T>::functionCache_t::
      const_iterator it = cache_[0].find (argument);
    if (it != cache_[0].end ())
      {
        result = *(it->second);
        return;
      }
    (*function_) (result, argument);

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    bool cur_malloc_allowed = is_malloc_allowed ();
    set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

    cache_[0][argument] = result;

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
  }

  template <typename T>
  void
  CachedFunction<T>::impl_gradient (gradient_ref gradient,
                                    const_argument_ref argument,
                                    size_type functionId)
    const
  {
    cachedFunctionGradient<T> (gradient, argument, functionId);
  }


  template <typename T>
  void
    CachedFunction<T>::impl_jacobian
    (jacobian_ref jacobian,
      const_argument_ref argument) const
  {
    cachedFunctionJacobian<T>(jacobian, argument);
  }


  template <typename T>
  void
  CachedFunction<T>::impl_hessian (hessian_ref hessian,
  				   const_argument_ref argument,
  				   size_type functionId)
    const
  {
    cachedFunctionHessian<T> (hessian, argument, functionId);
  }


  template <typename T>
  void
  CachedFunction<T>::impl_derivative (gradient_ref derivative,
  				      value_type argument,
  				      size_type order)
    const
  {
    cachedFunctionDerivative<T> (derivative, argument, order);
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DECORATOR_CACHED_FUNCTION_HXX
