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

#include "shared-tests/fixture.hh"

#include <iostream>
#include <vector>
#include <math.h>

#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/format.hpp>

#include <Eigen/Geometry>

#include <roboptim/core/io.hh>
#include <roboptim/core/differentiable-function.hh>
#include <roboptim/core/function-pool.hh>
#include <roboptim/core/finite-difference-gradient.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

class Engine
{
public:

  typedef Eigen::Vector2d point_t;
  typedef Eigen::Rotation2Dd rot_t;

  struct EngineData
  {
    typedef std::vector<point_t> points_t;
    typedef std::vector<points_t> gradPoints_t;

    std::size_t n;
    double length;
    points_t points;
    gradPoints_t grad;

    EngineData (std::size_t N)
    : n (N),
      length (1.),
      points (N+1,
              point_t::Zero ()),
      grad (N, points_t (N+1, point_t::Zero ()))
    {
    }
  };

  typedef Function::const_argument_ref const_argument_ref;
  typedef EngineData data_t;

  Engine (std::size_t N) : data_ (N)
  {
  }

  virtual ~Engine () {}

  const data_t& data () const
  {
    return data_;
  }

  void compute (const_argument_ref x)
  {
    (*output) << "Engine: calling compute" << std::endl;

    point_t arm;
    arm << data_.length, 0.;
    rot_t rot (0.);

    // For each point
    for (std::size_t i = 0; i < data_.n; ++i)
    {
      point_t::Index ii = static_cast<point_t::Index> (i);
      rot *= rot_t (x[ii]);
      data_.points[i+1] = data_.points[i] + rot * arm;
    }
  }

  void jacobian (const_argument_ref x)
  {
    (*output) << "Engine: calling jacobian" << std::endl;

    point_t arm;
    arm << data_.length, 0.;

    // For each argument (q₀, q₁, q₂)
    for (std::size_t i = 0; i < data_.n; ++i)
    {
      rot_t rot (0.);
      rot_t grad_rot (0.);

      for (std::size_t j = 0; j <= i; ++j)
      {
        data_.grad[i][j].setZero ();
      }

      // For each point (P₁, P₂, P₃)
      for (std::size_t j = 0; j < data_.n; ++j)
      {
        point_t::Index jj = static_cast<point_t::Index> (j);
        rot *= rot_t (x[jj]);
        if (j >= i)
        {
          grad_rot = rot_t (M_PI/2.) * rot;
          data_.grad[i][j+1] = data_.grad[i][j] + grad_rot * arm;
        }
      }
    }
  }

private:
  data_t data_;
};


template <typename T>
class EngineWrapper : public GenericDifferentiableFunction<T>
{
public:
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  typedef Engine engine_t;
  typedef boost::shared_ptr<Engine> engine_ptr;

  EngineWrapper (engine_ptr engine)
    : GenericDifferentiableFunction<T>
      (static_cast<size_type> (engine->data ().n), 1, "engine"),
    engine_ (engine)
  {
  }

  void impl_compute (result_ref, const_argument_ref x) const
  {
    engine_->compute (x);
  }

  void impl_gradient (gradient_ref, const_argument_ref, size_type) const
  {
    assert (0);
  }

  void impl_jacobian (jacobian_ref, const_argument_ref x) const
  {
    engine_->jacobian (x);
  }

private:
  engine_ptr engine_;
};

template <typename T>
class Position : public GenericDifferentiableFunction<T>
{
public:
  ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
  (GenericDifferentiableFunction<T>);

  typedef boost::shared_ptr<Engine> engine_t;

  Position (const engine_t engine, std::size_t idx,
            const std::string& name)
    : GenericDifferentiableFunction<T>
        (static_cast<size_type> (engine->data ().n),
         2, name),
      engine_ (engine),
      idx_ (idx)
  {
  }

  void impl_compute (result_ref res, const_argument_ref) const
  {
    res = engine_->data ().points[1+idx_];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref, size_type dim) const
  {
    // For each argument
    for (size_type i = 0; i < this->inputSize (); ++i)
      grad[i] = engine_->data ().grad[static_cast<std::size_t> (i)][1+idx_][dim];
  }

private:
  engine_t engine_;
  std::size_t idx_;
};

template <>
void Position<EigenMatrixSparse>::impl_gradient
(gradient_ref grad, const_argument_ref, size_type dim) const
{
  // For each argument
  for (size_type i = 0; i < this->inputSize (); ++i)
    grad.coeffRef (i) = engine_->data ().grad[static_cast<std::size_t> (i)][1+idx_][dim];
}


inline GenericFunctionTraits<EigenMatrixDense>::matrix_t
to_dense (const GenericFunctionTraits<EigenMatrixDense>::matrix_t& m)
{
  return m;
}

inline GenericFunctionTraits<EigenMatrixDense>::matrix_t
to_dense (const GenericFunctionTraits<EigenMatrixSparse>::matrix_t& m)
{
  return sparse_to_dense (m);
}

struct PositionConstraintVisitor : public boost::static_visitor<>
{
  typedef Function::const_argument_ref const_argument_ref;

  PositionConstraintVisitor (const_argument_ref x)
    : x_ (x)
  {}

  template <typename F>
  void operator () (const F& f)
  {
    (*output) << (*f) << std::endl;
    (*output) << (*f) (x_) << std::endl;
    (*output) << to_dense (f->jacobian (x_)) << std::endl;
  }

  private:
    const_argument_ref x_;
};

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

BOOST_AUTO_TEST_CASE_TEMPLATE (function_pool, T, functionTypes_t)
{
  output = retrievePattern ("function-pool");

  size_t n_joints = 3;
  boost::shared_ptr<Engine> engine = boost::make_shared<Engine> (n_joints);
  boost::shared_ptr<EngineWrapper<T> >
    engine_wrapper = boost::make_shared<EngineWrapper<T> > (engine);

  typedef boost::mpl::list<GenericDifferentiableFunction<T> > poolTypes_t;
  typedef GenericDifferentiableFunction<T> poolFunction_t;
  typedef FunctionPool<poolFunction_t, poolTypes_t> pool_t;
  typedef typename poolFunction_t::result_t result_t;
  typedef typename poolFunction_t::jacobian_t jacobian_t;

  typename pool_t::functionList_t position_vector;

  for (size_t i = 0; i < n_joints; ++i)
  {
    std::string name = (boost::format ("Position of joint %1%") % i).str ();
    position_vector.push_back (boost::make_shared<Position<T> > (engine, i, name));
  }

  typename Position<T>::argument_t x (static_cast<typename pool_t::size_type> (n_joints));
  x[0] =  M_PI/4.;
  x[1] = -M_PI/4.;
  x[2] =  M_PI/2.;

  boost::shared_ptr<pool_t> pool
    = boost::make_shared<pool_t> (engine_wrapper, position_vector,
                                  "Joint position pool");

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    Eigen::internal::set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

  // Pre-allocate memory
  result_t res (pool->outputSize ());
  res.setZero ();
  jacobian_t jac (pool->outputSize (), pool->inputSize ());
  jac.setZero ();
  jacobian_t fd_jac (pool->outputSize (), pool->inputSize ());
  fd_jac.setZero ();

  GenericFunctionTraits<EigenMatrixDense>::matrix_t
    denseJac (pool->outputSize (), pool->inputSize ());
  GenericFunctionTraits<EigenMatrixDense>::matrix_t
    fd_denseJac (pool->outputSize (), pool->inputSize ());

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
  Eigen::internal::set_is_malloc_allowed (false);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

  // Call the pool
  (*pool) (res, x);
  pool->jacobian (jac, x);

  // Compare gradient with finite differences
  GenericFiniteDifferenceGradient<T> fd_pool (*pool);
  fd_pool.jacobian (fd_jac, x);

  denseJac = to_dense (jac);

  // Print the result
  (*output) << "x = " << x << std::endl;
  (*output) << *pool << std::endl;
  (*output) << res << std::endl;
  (*output) << denseJac << std::endl;

  // Check the individual functions
  PositionConstraintVisitor visitor (x);
  for (typename pool_t::functionList_t::const_iterator
       iter  = position_vector.begin ();
       iter != position_vector.end (); ++iter)
  {
    boost::apply_visitor (visitor, *iter);
  }

  fd_denseJac = to_dense (fd_jac);
  (*output) << fd_denseJac << std::endl;

  BOOST_CHECK (allclose (jac, fd_jac, 1e-6, 1e-6));

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
