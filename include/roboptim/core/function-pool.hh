// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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

#ifndef ROBOPTIM_CORE_FUNCTION_POOL_HH
# define ROBOPTIM_CORE_FUNCTION_POOL_HH

# pragma message "FunctionPool is an experimental feature likely to evolve."

# include <vector>

# include <boost/shared_ptr.hpp>
# include <boost/mpl/assert.hpp>
# include <boost/mpl/logical.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <roboptim/core/differentiable-function.hh>
# include <roboptim/core/detail/utility.hh>

namespace roboptim
{

  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief A pool of functions that will be processed together.
  ///
  /// This class provides a way to regroup functions that depend on the same
  /// "computation engine", for instance a simulator that generates some data,
  /// then each function of the pool can simply read the computed data (which
  /// could be done in parallel).
  ///
  /// TODO: the actual type of the FunctionPool should depend on the list of
  /// constraints that are supported. We could imagine using a metaprogramming
  /// version of LCA (lowest common ancestor) to find the Function type that
  /// will best match the list. In hybrid scenarios where we have both
  /// differentiable and non-differentiable functions, two pools sharing the
  /// same callback should probably be used.  For now, we will rely on a
  /// user-provided function type.
  ///
  /// \tparam F type of the pool.
  /// \tparam FLIST list of valid function types.
  template <typename F, typename FLIST>
  class FunctionPool : public F
  {
    // Check that F derives from Function or SparseFunction.
    BOOST_MPL_ASSERT_MSG((detail::derives_from_function<F>::type::value),
                         ROBOPTIM_FUNCTION_EXPECTED_FOR_POOL, (F&));

  public:
    /// \brief Function type of the pool.
    /// Note: this will be set by LCA called on the function type list.
    typedef F pool_t;

    typedef typename detail::list_converter<FLIST>::type functionTypeList_t;

    /// \brief Type of the functions in the pool.
    typedef typename detail::shared_ptr_variant<functionTypeList_t>::type
    function_t;

    /// \brief List of functions that will populate the pool.
    typedef std::vector<function_t> functionList_t;

    /// \brief Type of the callback function (where the computation happens).
    typedef F callback_t;
    typedef boost::shared_ptr<callback_t> callback_ptr;

    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_ (pool_t);

    /// \brief FunctionPool constructor.
    ///
    /// \param callback  callback function for the engine.
    /// \param functions functions of the pool.
    /// \param name      name of the pool.
    explicit FunctionPool (const callback_ptr callback,
                           const functionList_t& functions,
                           const std::string& name = "");

    /// \brief Virtual destructor.
    virtual ~FunctionPool ();


    virtual void impl_compute (result_ref result, const_argument_ref x) const;

    virtual void impl_gradient (gradient_ref gradient,
                                const_argument_ref argument,
                                size_type functionId = 0) const;

    virtual void impl_jacobian (jacobian_ref jacobian,
                                const_argument_ref arg) const;

    /// \brief Overriden print function for pools.
    virtual std::ostream& print (std::ostream&) const;

    /// \brief Get the input size from the function list.
    static size_type listInputSize (const functionList_t& functions);

    /// \brief Get the output size from the function list.
    static size_type listOutputSize (const functionList_t& functions);

  private:
    /// \brief Functions of the pool.
    functionList_t functions_;

    /// \brief Callback for the engine where computation happens.
    callback_ptr callback_;

    /// \brief Dummy vector to avoid callback allocations.
    mutable typename callback_t::result_t callback_res_;

    /// \brief Dummy matrix to avoid callback allocations.
    mutable typename callback_t::jacobian_t callback_jac_;
  };

  /// @}

} // end of namespace roboptim

# include <roboptim/core/function-pool.hxx>

#endif //! ROBOPTIM_CORE_FUNCTION_POOL_HH
