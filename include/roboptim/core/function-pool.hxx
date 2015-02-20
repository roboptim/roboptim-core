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

# include <boost/variant/apply_visitor.hpp>
# include <boost/utility/enable_if.hpp>

# include <roboptim/core/util.hh>

namespace roboptim
{
  namespace
  {
    struct PoolInputSizeVisitor : public boost::static_visitor<std::size_t>
    {
      PoolInputSizeVisitor (){}

      template <typename F>
      std::size_t operator () (const F& f)
      {
        return static_cast<std::size_t> (f->inputSize ());
      }
    };

    template <typename F, typename FLIST>
    typename F::size_type
    poolInputSize (const typename FunctionPool<F,FLIST>::functionList_t& functions)
    {
      PoolInputSizeVisitor visitor;
      std::size_t inputSize = boost::apply_visitor (visitor, functions[0]);

      // All functions should have the same input size
      for (std::size_t i = 1; i < functions.size (); ++i)
        assert (inputSize == boost::apply_visitor (visitor, functions[i]));

      return static_cast<typename F::size_type> (inputSize);
    }

    struct PoolOutputSizeVisitor : public boost::static_visitor<std::size_t>
    {
      PoolOutputSizeVisitor (){}

      template <typename F>
      std::size_t operator () (const F& f)
      {
        return static_cast<std::size_t> (f->outputSize ());
      }
    };

    template <typename F, typename FLIST>
    typename F::size_type
    poolOutputSize (const typename FunctionPool<F,FLIST>::functionList_t& functions)
    {
      typename F::size_type outputSize = 0;
      PoolOutputSizeVisitor visitor;

      for (std::size_t i = 0; i < functions.size (); ++i)
        outputSize += static_cast<typename F::size_type> (boost::apply_visitor (visitor, functions[i]));

      return outputSize;
    }

    template <typename F>
    struct PoolComputeVisitor : public boost::static_visitor<void>
    {
      PoolComputeVisitor (typename F::result_ref result,
                          typename F::const_argument_ref x)
	: result_ (result),
	  x_ (x),
	  idx_ (0)
      {}

      template <typename U>
      void operator () (const boost::shared_ptr<U>& f)
      {
        result_.segment (idx_, f->outputSize ()) = (*f)(x_);
        idx_ += f->outputSize ();
      }

    private:
      typename F::result_ref result_;
      typename F::const_argument_ref x_;
      typename F::size_type idx_;
    };

    template <typename F>
    struct PoolJacobianVisitor : public boost::static_visitor<void>
    {
      PoolJacobianVisitor (typename F::jacobian_ref jacobian,
                           typename F::const_argument_ref x)
	: jacobian_ (jacobian),
	  x_ (x),
	  idx_ (0),
	  m_ (jacobian.cols ())
      {}

      template <typename U>
      void operator ()
      (const boost::shared_ptr<U>& f,
       typename boost::enable_if<boost::is_same<typename U::traits_t,
                                                EigenMatrixDense>
                                >::type* = 0)
      {
        assert (f->inputSize () == m_);

        jacobian_.block (idx_, 0, f->outputSize (), m_)
          = f->jacobian (x_);
        idx_ += f->outputSize ();
      }

      template <typename U>
      void operator ()
      (const boost::shared_ptr<U>& f,
       typename boost::disable_if<boost::is_same<typename U::traits_t,
                                                 EigenMatrixDense>
                                 >::type* = 0)
      {
        assert (f->inputSize () == m_);

        // TODO: update once sparse submatrix support makes it possible
        copySparseBlock (jacobian_, f->jacobian (x_), idx_, 0);
        idx_ += f->outputSize ();
      }

    private:
      typename F::jacobian_ref jacobian_;
      typename F::const_argument_ref x_;
      typename F::size_type idx_;
      typename F::size_type m_;
    };

    template <typename F>
    struct PoolPrintVisitor : public boost::static_visitor<void>
    {
      PoolPrintVisitor (std::ostream& o)
        : o_ (o)
      {}

      template <typename U>
      void operator () (const boost::shared_ptr<U>& f)
      {
        o_ << iendl << *f;
      }

    private:
      std::ostream& o_;
    };
  } // end of unnamed namespace


  template <typename F, typename FLIST>
  FunctionPool<F,FLIST>::FunctionPool (const callback_ptr callback,
                                       const functionList_t& functions,
                                       const std::string& name)
    : pool_t (poolInputSize<F,FLIST> (functions),
	      poolOutputSize<F,FLIST> (functions),
	      (name == "") ? "Function pool" : name),
      functions_ (functions),
      callback_ (callback)
  {
  }

  template <typename F, typename FLIST>
  FunctionPool<F,FLIST>::~FunctionPool ()
  {
  }

  template <typename F, typename FLIST>
  void FunctionPool<F,FLIST>::impl_compute (result_ref result, const_argument_ref x)
    const
  {
    // First, run the engine (callback)
    (*callback_) (x);

    // Second, process the functions of the pool.
    // Note: here a dummy x would work just as well since the computation
    // should have been done in the engine already.
    // TODO: use a fake argument vector that triggers an error when accessed
    // to detect any misuse of the pool?
    typedef typename functionList_t::const_iterator citer_t;
    PoolComputeVisitor<pool_t> visitor (result, x);
    for (citer_t f = functions_.begin (); f != functions_.end (); ++f)
      {
	boost::apply_visitor (visitor, *f);
      }
  }

  template <typename F, typename FLIST>
  void FunctionPool<F,FLIST>::impl_gradient (gradient_ref,
                                             const_argument_ref,
                                             size_type)
    const
  {
    // For performance and ease of use, we compute the whole jacobian
    // in one method.
    assert (0);
  }

  template <typename F, typename FLIST>
  void FunctionPool<F,FLIST>::impl_jacobian (jacobian_ref jacobian,
                                             const_argument_ref x) const
  {
    // Initialize the Jacobian
    jacobian.setZero ();

    // First, run the engine (callback)
    callback_->jacobian (x);

    // Second, process the functions of the pool.
    // Note: here a dummy x would work just as well since the computation
    // should have been done in the engine already.
    // TODO: use a fake argument vector that triggers an error when accessed
    // to detect any misuse of the pool?
    typedef typename functionList_t::const_iterator citer_t;
    PoolJacobianVisitor<pool_t> visitor (jacobian, x);
    for (citer_t f = functions_.begin (); f != functions_.end (); ++f)
      {
	boost::apply_visitor (visitor, *f);
      }
  }

  template <typename F, typename FLIST>
  std::ostream& FunctionPool<F,FLIST>::print (std::ostream& o) const
  {
    typedef typename functionList_t::const_iterator citer_t;
    PoolPrintVisitor<pool_t> visitor (o);

    parent_t::print (o) << ":" << incindent;

    for (citer_t f = functions_.begin (); f != functions_.end (); ++f)
      {
	boost::apply_visitor (visitor, *f);
      }

    o << decindent;
    return o;
  }

  template <typename F, typename FLIST>
  typename FunctionPool<F,FLIST>::size_type FunctionPool<F,FLIST>::inputSize
  (const typename FunctionPool<F,FLIST>::functionList_t& functions)
  {
    return poolInputSize<F,FLIST> (functions);
  }

  template <typename F, typename FLIST>
  typename FunctionPool<F,FLIST>::size_type FunctionPool<F,FLIST>::outputSize
  (const typename FunctionPool<F,FLIST>::functionList_t& functions)
  {
    return poolOutputSize<F,FLIST> (functions);
  }
} // end of namespace roboptim
