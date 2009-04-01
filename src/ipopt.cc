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
#include "problem.hh"
#include "util.hh"

namespace optimization
{
  using namespace Ipopt;

  namespace detail
  {
    /// Ipopt non linear problem definition.
    struct MyTNLP : public TNLP
    {
      MyTNLP (IpoptSolver& solver)
        throw ()
        : solver_ (solver)
      {}

      virtual bool
      get_nlp_info (Index& n, Index& m, Index& nnz_jac_g,
                    Index& nnz_h_lag, TNLP::IndexStyleEnum& index_style)
        throw ()
      {
        n = solver_.problem.function.n;
        m = solver_.problem.constraints.size ();
        nnz_jac_g = n * m; //FIXME: use a dense matrix for now.
        nnz_h_lag = n * n; //FIXME: use a dense matrix for now.
        index_style = TNLP::C_STYLE;
        return true;
      }
      virtual bool
      get_bounds_info (Index n, Number* x_l, Number* x_u,
                       Index m, Number* g_l, Number* g_u)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

        {
          typedef Function::bounds_t::const_iterator citer_t;
          for (citer_t it = solver_.problem.function.argBounds.begin ();
               it != solver_.problem.function.argBounds.end (); ++it)
            *(x_l++) = (*it).first, *(x_u++) = (*it).second;
        }

        {
          typedef Problem::constraints_t::const_iterator citer_t;
          for (citer_t it = solver_.problem.constraints.begin ();
               it != solver_.problem.constraints.end (); ++it)
            *(g_l++) = (*it)->bound.first, *(g_u++) = (*it)->bound.second;
        }
        return true;
      }

      virtual bool
      get_starting_point(Index n, bool init_x, Number* x,
                         bool init_z, Number* z_L, Number* z_U,
                         Index m, bool init_lambda,
                         Number* lambda)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

        //FIXME: handle all modes.
        assert(init_x == true);
        assert(init_z == false);
        assert(init_lambda == false);

        if (solver_.problem.start.empty () && !!init_x)
          solver_.result_ =
            SolverError ("Ipopt method needs a starting point.");
        if (solver_.problem.start.empty ())
          return true;

        vector_to_array (x, solver_.problem.start);
        return true;
      }

      virtual bool
      eval_f (Index n, const Number* x, bool new_x, Number& obj_value)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);
        obj_value = solver_.problem.function (x_);
        return true;
      }

      virtual bool
      eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);
        Function::gradient_t grad = solver_.problem.function.gradient (x_);
        if (!grad)
          return false;
        vector_to_array(grad_f, *grad);
        return true;
      }

      virtual bool
      eval_g(Index n, const Number* x, bool new_x,
             Index m, Number* g)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);

        typedef Problem::constraints_t::const_iterator citer_t;

        IpoptSolver::vector_t g_ (m);
        int i = 0;
        for (citer_t it = solver_.problem.constraints.begin ();
             it != solver_.problem.constraints.end (); ++it, ++i)
          g_[i] = (**it) (x_);
        vector_to_array(g, g_);
        return true;
      }

      virtual bool
      eval_jac_g(Index n, const Number* x, bool new_x,
                 Index m, Index nele_jac, Index* iRow,
                 Index *jCol, Number* values)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

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
            IpoptSolver::vector_t x_ (n);
            array_to_vector (x_, x);
            Function::matrix_t jac (solver_.problem.constraints.size (),
                                    solver_.problem.function.n);
            jacobian_from_gradients (jac, solver_.problem.constraints, x_);

            int idx = 0;
            for (int i = 0; i < m; ++i)
              for (int j = 0; j < n; ++j)
                values[idx++] = jac (i, j);
          }

        return true;
      }

      /// Compute Ipopt hessian from several hessians.
      void compute_hessian (Function::matrix_t& h,
                            const IpoptSolver::vector_t& x,
                            Number obj_factor,
                            const Number* lambda)

      {
        typedef Problem::constraints_t::const_iterator citer_t;

        Function::hessian_t fct_h = solver_.problem.function.hessian (x);
        h = obj_factor * *fct_h;

        int i = 0;
        for (citer_t it = solver_.problem.constraints.begin ();
             it != solver_.problem.constraints.end (); ++it)
          h += lambda[i++] * *((*it)->hessian (x));
      }

      virtual bool
      eval_h (Index n, const Number* x, bool new_x,
              Number obj_factor, Index m, const Number* lambda,
              bool new_lambda, Index nele_hess, Index* iRow,
              Index* jCol, Number* values)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

        //FIXME: check if a hessian is provided.

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
            assert(idx == nele_hess);
          }
        else
          {
            IpoptSolver::vector_t x_ (n);
            array_to_vector (x_, x);

            IpoptSolver::vector_t lambda_ (m);
            array_to_vector (lambda_, lambda);

            Function::matrix_t h (solver_.problem.function.n,
                                     solver_.problem.function.n);
            compute_hessian (h, x_, obj_factor, lambda);

            int idx = 0;
            for (int i = 0; i < n; ++i)
              for (int j = 0; j < n; ++j)
                values[idx++] = h (i, j);
          }

        return true;
      }

      virtual void
      finalize_solution(SolverReturn status,
                        Index n, const Number* x, const Number* z_L,
                        const Number* z_U, Index m, const Number* g,
                        const Number* lambda, Number obj_value,
                        const IpoptData* ip_data,
                        IpoptCalculatedQuantities* ip_cq)
        throw ()
      {
        assert (solver_.problem.function.n - n == 0);
        assert (solver_.problem.constraints.size () - m == 0);

        if (status != SUCCESS)
          {
            //FIXME: put a more precise error message.
            solver_.result_ = SolverError ("Ipopt failed to minimize.");
            return;
          }

        Solver::vector_t arr (n);
        array_to_vector (arr, x);
        solver_.result_ = arr;
      }

      IpoptSolver& solver_;
    };
  }; // end of namespace detail

  using namespace detail;

  IpoptSolver::IpoptSolver (const Problem& pb) throw ()
    : Solver (pb),
      nlp_ (new MyTNLP (*this)),
      app_ (new IpoptApplication (false, false))
  {
    // Set default options.
    app_->Options ()->SetNumericValue ("tol", 1e-7);
    app_->Options ()->SetStringValue ("mu_strategy", "adaptive");
    app_->Options ()->SetStringValue ("output_file", "");
  }

  IpoptSolver::~IpoptSolver () throw ()
  {
  }

  IpoptSolver::result_t
  IpoptSolver::getMinimum () throw ()
  {
    if (result_.which () != SOLVER_NO_SOLUTION)
      return result_;

    ApplicationReturnStatus status = app_->Initialize ("");
    if (status != Solve_Succeeded)
      {
        //FIXME: put a more precise error message.
        result_ = SolverError ("Ipopt application failed to initialize.");
        return result_;
      }
    app_->OptimizeTNLP (nlp_);
    return result_;
  }

  Ipopt::SmartPtr<Ipopt::IpoptApplication>
  IpoptSolver::getApplication () throw ()
  {
    return app_;
  }

} // end of namespace optimization
