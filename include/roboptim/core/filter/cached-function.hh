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

#ifndef ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HH
# define ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <map>

# include <boost/shared_ptr.hpp>

# include <roboptim/core/n-times-derivable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_filter
  /// @{

  namespace detail
  {
    struct ltvector
    {
      bool operator()(const Function::vector_t& v1,
		      const Function::vector_t& v2) const
      {
	Function::size_type it1 = 0;
	Function::size_type it2 = 0;

	while (it1 != v1.size () && it2 != v2.size ())
	  {
	    if (fabs (v1(it1) - v2(it2)) < Function::epsilon ())
	      ++it1, ++it2;
	    else if (v1(it1) - v2(it2) < - Function::epsilon ())
	      return true;
	    else return false;
	  }
	return false;
      }
    };

  } // end of namespace detail.


  /// \brief Store previous function computation.
  ///
  /// When an expensive function is called several times at the same
  /// point (exactly!), the cached function prevents useless
  /// computation by caching the function result.
  ///
  /// This filter is experimental in this release.
  template <typename T>
  class CachedFunction : public T
  {
  public:
    /// \brief Import traits type.
    typedef typename T::traits_t traits_t;
    /// \brief Import value type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    value_type value_type;
    /// \brief Import size type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    size_type size_type;
    /// \brief Import vector type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    vector_t vector_t;
    /// \brief Import result type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    result_t result_t;
    /// \brief Import argument type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    argument_t argument_t;
    /// \brief Import gradient type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    gradient_t gradient_t;
    /// \brief Import hessian type.
    typedef typename GenericTwiceDifferentiableFunction<traits_t>::
    hessian_t hessian_t;
    /// \brief Import jacobian type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    jacobian_t jacobian_t;
    /// \brief Import interval type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    interval_t interval_t;


    typedef std::map<argument_t, vector_t, detail::ltvector>
    functionCache_t;
    typedef std::map<argument_t, gradient_t, detail::ltvector>
    gradientCache_t;
    typedef std::map<argument_t, jacobian_t, detail::ltvector>
    jacobianCache_t;
    typedef std::map<argument_t, hessian_t, detail::ltvector>
    hessianCache_t;

    explicit CachedFunction (boost::shared_ptr<const T> fct);
    ~CachedFunction ();

    /// \brief Reset the caches.
    void reset ();

  protected:
    virtual void impl_compute (result_t& result, const argument_t& argument)
      const;


    virtual void impl_gradient (gradient_t& gradient,
				const argument_t& argument,
				size_type functionId = 0)
      const;

    virtual void impl_jacobian (jacobian_t& jacobian, const argument_t& arg)
      const;

    virtual void impl_hessian (hessian_t& hessian,
    			       const argument_t& argument,
    			       size_type functionId = 0) const;

    virtual void impl_derivative (gradient_t& derivative,
    				  value_type argument,
    				  size_type order = 1) const;

  protected:
    boost::shared_ptr<const T> function_;
    mutable std::vector<functionCache_t> cache_;
    mutable std::vector<gradientCache_t> gradientCache_;
    mutable jacobianCache_t jacobianCache_;
    mutable std::vector<hessianCache_t> hessianCache_;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/filter/cached-function.hxx>
#endif //! ROBOPTIM_CORE_FILTER_CACHED_FUNCTION_HH
