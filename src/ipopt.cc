// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file src/ipopt.cc
 *
 * \brief Implementation of the Ipopt class.
 */

#include <cassert>

#include <IpIpoptApplication.hpp>
#include <IpTNLP.hpp>

#include "ipopt.hh"

namespace optimization
{
  using namespace Ipopt;

  namespace detail
  {
    //FIXME: check if this is safe.
    static void
    vector_to_array (Solver::value_type* dst, const Solver::array_t& src)
    {
      memcpy (dst, &src, src.size () * sizeof (Solver::value_type));
    }

    static void
    array_to_vector (Solver::array_t& dst, const Solver::value_type* src)
    {
      memcpy (&dst[0], src, dst.size () * sizeof (Solver::value_type));
    }

    /// Ipopt non linear problem definition.
    struct MyTNLP : public TNLP
    {
      MyTNLP (IpoptSolver& solver)
        : solver_ (solver)
      {}

      virtual bool
      get_nlp_info (Index& n, Index& m, Index& nnz_jac_g,
                    Index& nnz_h_lag, TNLP::IndexStyleEnum& index_style)
      {
        n = solver_.getArity ();
        m = solver_.getConstraints ().size ();
        nnz_jac_g = n * m; //FIXME: use a dense matrix for now.
        nnz_h_lag = n * n; //FIXME: use a dense matrix for now.
        index_style = TNLP::C_STYLE;
        return true;
      }
      virtual bool
      get_bounds_info (Index n, Number* x_l, Number* x_u,
                       Index m, Number* g_l, Number* g_u)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        {
          typedef IpoptSolver::bounds_t::const_iterator citer_t;
          for (citer_t it = solver_.bounds_.begin ();
               it != solver_.bounds_.end (); ++it)
            *(x_l++) = (*it).first, *(x_u++) = (*it).second;
        }

        {
          typedef IpoptSolver::constraints_t::const_iterator citer_t;
          for (citer_t it = solver_.getConstraints ().begin ();
               it != solver_.getConstraints ().end (); ++it)
            *(g_l++) = (*it).lower, *(g_u++) = (*it).upper;
        }
        return true;
      }

      virtual bool
      get_starting_point(Index n, bool init_x, Number* x,
                         bool init_z, Number* z_L, Number* z_U,
                         Index m, bool init_lambda,
                         Number* lambda)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        //FIXME: handle all modes.
        assert(init_x == true);
        assert(init_z == false);
        assert(init_lambda == false);

        if (!solver_.start_ && !!init_x)
          solver_.result_ = SolverError ();
        if (!solver_.start_)
          return true;

        vector_to_array (x, *solver_.start_);
        return true;
      }

      virtual bool
      eval_f (Index n, const Number* x, bool new_x, Number& obj_value)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);

        IpoptSolver::array_t x_ (n);
        array_to_vector (x_, x);
        obj_value = solver_.getFunction () (x_);
        return true;
      }

      virtual bool
      eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);

        if (!solver_.getGradient ())
          return false;

        IpoptSolver::array_t x_ (n);
        array_to_vector (x_, x);
        IpoptSolver::array_t grad = (*solver_.getGradient ()) (x_);
        vector_to_array(grad_f, grad);
        return true;
      }

      virtual bool
      eval_g(Index n, const Number* x, bool new_x,
             Index m, Number* g)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        IpoptSolver::array_t x_ (n);
        array_to_vector (x_, x);

        typedef IpoptSolver::constraints_t::const_iterator citer_t;

        IpoptSolver::array_t g_ (m);
        int i = 0;
        for (citer_t it = solver_.getConstraints ().begin ();
             it != solver_.getConstraints ().end (); ++it, ++i)
          g_[i] = (*it).function (x_);
        vector_to_array(g, g_);
        return true;
      }

      virtual bool
      eval_jac_g(Index n, const Number* x, bool new_x,
                 Index m, Index nele_jac, Index* iRow,
                 Index *jCol, Number* values)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        if (!solver_.getJacobian ())
          return false;

        if (!values)
          {
            //FIXME: always dense for now.
            int idx = 0;
            for (int i = 0; i < m; ++i)
              for (int j = 0; j < n; ++j)
                {
                  iRow[idx] = i, jCol[idx] = j;
                  ++idx;
                }
          }
        else
          {
            //FIXME: implement me.
            IpoptSolver::array_t x_ (n);
            array_to_vector (x_, x);
            IpoptSolver::matrix_t jac =
              (*solver_.getJacobian ()) (x_, solver_.getConstraints ());

            int idx = 0;
            for (int i = 0; i < m; ++i)
              for (int j = 0; j < n; ++j)
                values[idx++] = jac (i, j);
          }

        return true;
      }

      virtual bool
      eval_h (Index n, const Number* x, bool new_x,
              Number obj_factor, Index m, const Number* lambda,
              bool new_lambda, Index nele_hess, Index* iRow,
              Index* jCol, Number* values)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        if (!solver_.getHessian ())
          return false;

        if (!values)
          {
            //FIXME: always dense for now.
            int idx = 0;
            for (int i = 0; i < n; ++i)
              for (int j = 0; j < n; ++j)
                {
                  iRow[idx] = i, jCol[idx] = j;
                  ++idx;
                }
          }
        else
          {
            //FIXME: implement me.
            IpoptSolver::array_t x_ (n);
            array_to_vector (x_, x);

            IpoptSolver::array_t lambda_ (m);
            array_to_vector (lambda_, lambda);

            IpoptSolver::matrix_t h =
              (*solver_.getHessian ()) (x_, solver_.getConstraints (),
                                        obj_factor, lambda_);

            int idx = 0;
            for (int i = 0; i < n; ++i)
              for (int j = 0; j < n; ++j)
                values[idx++] = h (i, j);
          }

        return true;
      }

      virtual void
      finalize_solution(SolverReturn status,
                        Index n, const Number* x, const Number* z_L, const Number* z_U,
                        Index m, const Number* g, const Number* lambda,
                        Number obj_value,
                        const IpoptData* ip_data,
                        IpoptCalculatedQuantities* ip_cq)
      {
        assert (solver_.getArity () - n == 0);
        assert (solver_.bounds_.size () - n == 0);
        assert (solver_.getConstraints ().size () - m == 0);

        if (status != SUCCESS)
          {
            solver_.result_ = SolverError ();
            return;
          }

        Solver::array_t arr (n);
        array_to_vector (arr, x);
        solver_.result_ = arr;
      }

      IpoptSolver& solver_;
    };
  }; // end of namespace detail

  using namespace detail;

  IpoptSolver::IpoptSolver (function_t fct,
                            size_type n,
                            gradient_t g,
                            hessian_t h,
                            jacobian_t j) throw ()
    : Solver (fct, n, g, h, j),
      nlp_ (new MyTNLP (*this)),
      app_ (new IpoptApplication ())
  {
    // Set default options.
    app_->Options ()->SetNumericValue ("tol", 1e-7);
    app_->Options ()->SetStringValue ("mu_strategy", "adaptive");
    app_->Options ()->SetStringValue ("output_file", "ipopt.out");
  }

  IpoptSolver::~IpoptSolver () throw ()
  {
  }

  IpoptSolver::result_t
  IpoptSolver::getMinimum () throw ()
  {
    if (result_.which () != SOLVER_NO_SOLUTION)
      return result_;

    ApplicationReturnStatus status = app_->Initialize ();
    if (status != Solve_Succeeded)
      {
        result_ = SolverError ();
        return result_;
      }
    app_->OptimizeTNLP (nlp_);
    return result_;
  }

} // end of namespace optimization
