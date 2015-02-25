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
# include <boost/functional/hash.hpp>

# include <roboptim/core/n-times-derivable-function.hh>
# include <roboptim/core/cache.hh>

namespace Eigen
{
  /// \brief Hash function for argument vectors.
  /// \param x argument vector.
  /// \return hash.
  inline std::size_t hash_value (roboptim::Function::const_argument_ref x)
  {
    return boost::hash_range (x.data (), x.data () + x.size ());
  }
} // end of namespace Eigen

namespace roboptim
{
  /// \addtogroup roboptim_filter
  /// @{

  /// \brief Store previous function computation.
  ///
  /// When an expensive function is called several times at the same
  /// point (exactly!), the cached function prevents useless
  /// computation by caching the function result.
  ///
  /// This filter is experimental in this release.
  /// \tparam T input function type.
  template <typename T>
  class CachedFunction : public T
  {
  public:
    /// \brief Import traits type.
    typedef typename T::traits_t traits_t;

    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericTwiceDifferentiableFunction<traits_t>);

    /// \brief Import interval type.
    typedef typename GenericDifferentiableFunction<traits_t>::
    interval_t interval_t;

    /// \brief Key type for the cache.
    typedef argument_t cacheKey_t;

    typedef LRUCache<cacheKey_t, vector_t>   functionCache_t;
    typedef LRUCache<cacheKey_t, gradient_t> gradientCache_t;
    typedef LRUCache<cacheKey_t, jacobian_t> jacobianCache_t;
    typedef LRUCache<cacheKey_t, hessian_t>  hessianCache_t;

    /// \brief Cache a RobOptim function.
    /// \param fct function to cache.
    /// \param size size of the LRU cache.
    explicit CachedFunction (boost::shared_ptr<const T> fct,
                             size_t size = 10);
    ~CachedFunction ();

    /// \brief Reset the caches.
    void reset ();

  protected:
    virtual void impl_compute (result_ref result, const_argument_ref argument)
      const;


    virtual void impl_gradient (gradient_ref gradient,
				const_argument_ref argument,
				size_type functionId = 0)
      const;

    virtual void impl_jacobian (jacobian_ref jacobian, const_argument_ref arg)
      const;

    virtual void impl_hessian (hessian_ref hessian,
    			       const_argument_ref argument,
    			       size_type functionId = 0) const;

    virtual void impl_derivative (gradient_ref derivative,
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
