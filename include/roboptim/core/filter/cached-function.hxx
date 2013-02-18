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

#ifndef ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HXX
# define ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HXX
# include <boost/format.hpp>

# include <roboptim/core/derivative-size.hh>

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

  template <typename T>
  CachedFunction<T>::CachedFunction (boost::shared_ptr<const T> fct) throw ()
    : T (fct->inputSize (), fct->outputSize (), cachedFunctionName (*fct)),
      function_ (fct),
      cache_ (derivativeSize<T>::value),
      gradientCache_ (static_cast<std::size_t> (fct->outputSize ())),
      hessianCache_ (static_cast<std::size_t> (fct->outputSize ()))
  {
  }

  template <typename T>
  CachedFunction<T>::~CachedFunction () throw ()
  {
  }

  template <typename T>
  void
  CachedFunction<T>::reset () throw ()
  {
    cache_.clear ();
    gradientCache_.clear ();
    hessianCache_.clear ();
  }


  template <typename T>
  void
  CachedFunction<T>::impl_compute (result_t& result,
				   const argument_t& argument)
    const throw ()
  {
    functionCache_t::const_iterator it = cache_[0].find (argument);
    if (it != cache_[0].end ())
      {
	result = it->second;
	return;
      }
    (*function_) (result, argument);
    cache_[0][argument] = result;
  }


  template <>
  void
  CachedFunction<Function>::impl_gradient (gradient_t&, const argument_t&, size_type)
    const throw ()
  {
    assert (0);
  }

  template <typename T>
  void
  CachedFunction<T>::impl_gradient (gradient_t& gradient,
				    const argument_t& argument,
				    size_type functionId)
    const throw ()
  {
    functionCache_t::const_iterator it =
      gradientCache_[static_cast<std::size_t> (functionId)].find (argument);
    if (it != gradientCache_[static_cast<std::size_t> (functionId)].end ())
      {
	gradient = it->second;
	return;
      }
    function_->gradient (gradient, argument, functionId);
    gradientCache_[static_cast<std::size_t> (functionId)][argument] = gradient;
  }




  template <>
  void
  CachedFunction<Function>::impl_hessian
  (hessian_t&, const argument_t&, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  CachedFunction<DifferentiableFunction>::impl_hessian
  (hessian_t&, const argument_t&, size_type) const throw ()
  {
    assert (0);
  }



  template <typename T>
  void
  CachedFunction<T>::impl_hessian (hessian_t& hessian,
  				   const argument_t& argument,
  				   size_type functionId)
    const throw ()
  {
    //FIXME: bug detected by Clang. To be fixed.
#ifdef ROBOPTIM_CORE_THIS_DOES_NOT_WORK
    functionCache_t::const_iterator it =
      hessianCache_[static_cast<std::size_t> (functionId)].find (argument);
    if (it != hessianCache_[functionId].end ())
      {
	hessian = it->second;
	return;
      }
#endif
    function_->hessian (hessian, argument, functionId);
    hessianCache_[static_cast<std::size_t> (functionId)][argument] = hessian;
  }


  template <>
  void
  CachedFunction<Function>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  CachedFunction<DifferentiableFunction>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  CachedFunction<TwiceDifferentiableFunction>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <typename T>
  void
  CachedFunction<T>::impl_derivative (gradient_t& derivative,
  				      double argument,
  				      size_type order)
    const throw ()
  {
    vector_t x (1);
    x[0] = argument;
    functionCache_t::const_iterator it = cache_[order].find (x);
    if (it != cache_[order].end ())
      {
	derivative = it->second;
	return;
      }
    function_->derivative (derivative, x, order);
    cache_[order][x] = derivative;
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HXX
