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
 * \brief Implementation of the Ipopt class.
 */

#include <cassert>
#include <cstring>

#include <coin/IpIpoptApplication.hpp>
#include <coin/IpTNLP.hpp>

#include "liboptimization/ipopt.hh"
#include "liboptimization/util.hh"

namespace optimization
{
  using namespace Ipopt;

  namespace detail
  {
    TNLP::LinearityType cfsqp_tag (const LinearFunction& f)
    {
      return TNLP::LINEAR;
    }

    TNLP::LinearityType cfsqp_tag (const Function& f)
    {
      return TNLP::NON_LINEAR;
    }

    /// \internal
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
        n = solver_.problem ().function ().n;
        m = solver_.problem ().constraints ().size ();
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
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

        typedef IpoptSolver::problem_t::bounds_t::const_iterator citer_t;
        for (citer_t it = solver_.problem ().argBounds ().begin ();
             it != solver_.problem ().argBounds ().end (); ++it)
          *(x_l++) = (*it).first, *(x_u++) = (*it).second;

        for (citer_t it = solver_.problem ().bounds ().begin ();
             it != solver_.problem ().bounds ().end (); ++it)
          *(g_l++) = (*it).first, *(g_u++) = (*it).second;
        return true;
      }

      virtual bool
      get_scaling_parameters (Number& obj_scaling,
                              bool& use_x_scaling, Index n,
                              Number* x_scaling,
                              bool& use_g_scaling, Index m,
                              Number* g_scaling)
        throw ()
      {
        use_x_scaling = true, use_g_scaling = true;

        memcpy (x_scaling, &solver_.problem ().argScales ()[0],
                n * sizeof (double));

        for (Index i = 0; i < m; ++i)
          g_scaling[i] = solver_.problem ().scales ()[i];

        return false;
      }

      virtual bool
      get_variables_linearity (Index n, LinearityType* var_types) throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);

        //FIXME: detect from problem.
        for (Index i = 0; i < n; ++i)
          var_types[i] = cfsqp_tag (solver_.problem ().function ());
        return true;
      }

      virtual bool
      get_function_linearity (Index m, LinearityType* const_types) throw ()
      {
        assert (solver_.problem ().constraints ().size () - m == 0);

        //FIXME: detect from problem.
        for (Index i = 0; i < m; ++i)
          const_types[i] = cfsqp_tag (*solver_.problem ().constraints ()[i]);
        return true;
      }

      virtual bool
      get_starting_point (Index n, bool init_x, Number* x,
                          bool init_z, Number* z_L, Number* z_U,
                          Index m, bool init_lambda,
                          Number* lambda)
        throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

        //FIXME: handle all modes.
        assert(init_lambda == false);

        // Set bound multipliers.
        if (init_z)
          {
            //FIXME: for now, if required, scale is one.
            //When do we need something else?
            for (Index i = 0; i < n; ++i)
              z_L[i] = 1., z_U[i] = 1.;
          }

        // Set the starting point.
        if (!solver_.problem ().startingPoint () && init_x)
          {
            solver_.result_ =
              SolverError ("Ipopt method needs a starting point.");
            return false;
          }
        if (!solver_.problem ().startingPoint ())
          return true;

        vector_to_array (x, *solver_.problem ().startingPoint ());
        return true;
      }

      virtual bool
      get_warm_start_iterate (IteratesVector& warm_start_iterate) throw ()
      {
        //FIXME: implement this.
        //IteratesVector is defined in src/Algorithm/IteratesVector.hpp
        //and is not distributed (not a distributed header).
        //Hence, seems difficult to manipulate this type.
        //Idea 1: offer the possibility either to retrive this data after
        //solving a problem.
        //Idea 2: creating this type manually from problem + rough guess
        //(or previous solution).
        return false;
      }

      virtual bool
      eval_f (Index n, const Number* x, bool new_x, Number& obj_value)
        throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);
        obj_value = solver_.problem ().function () (x_);
        return true;
      }

      virtual bool
      eval_grad_f (Index n, const Number* x, bool new_x, Number* grad_f)
        throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);
        DerivableFunction::gradient_t grad = solver_.problem ().function ().gradient (x_);
        vector_to_array(grad_f, grad);
        return true;
      }

      virtual bool
      eval_g (Index n, const Number* x, bool new_x,
              Index m, Number* g)
        throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

        IpoptSolver::vector_t x_ (n);
        array_to_vector (x_, x);

        typedef IpoptSolver::problem_t::constraints_t::const_iterator citer_t;

        IpoptSolver::vector_t g_ (m);
        int i = 0;
        for (citer_t it = solver_.problem ().constraints ().begin ();
             it != solver_.problem ().constraints ().end (); ++it, ++i)
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
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

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
            Function::matrix_t jac (solver_.problem ().constraints ().size (),
                                    solver_.problem ().function ().n);
            jacobian_from_gradients<TwiceDerivableFunction> (jac, solver_.problem ().constraints (), x_);

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
        throw ()
      {
        typedef IpoptSolver::problem_t::constraints_t::const_iterator citer_t;

        TwiceDerivableFunction::hessian_t fct_h =
          solver_.problem ().function ().hessian (x);
        h = obj_factor * fct_h;

        int i = 0;
        for (citer_t it = solver_.problem ().constraints ().begin ();
             it != solver_.problem ().constraints ().end (); ++it)
          h += lambda[i++] * (*it)->hessian (x);
      }

      virtual bool
      eval_h (Index n, const Number* x, bool new_x,
              Number obj_factor, Index m, const Number* lambda,
              bool new_lambda, Index nele_hess, Index* iRow,
              Index* jCol, Number* values)
        throw ()
      {
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

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

            Function::matrix_t h (solver_.problem ().function ().n,
                                     solver_.problem ().function ().n);
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
        assert (solver_.problem ().function ().n - n == 0);
        assert (solver_.problem ().constraints ().size () - m == 0);

        if (status != SUCCESS)
          {
            //FIXME: put a more precise error message.
            solver_.result_ = SolverError ("Ipopt failed to minimize.");
            return;
          }

        IpoptSolver::vector_t arr (n);
        array_to_vector (arr, x);
        solver_.result_ = arr;
      }

      virtual bool
      intermediate_callback (AlgorithmMode mode,
                             Index iter, Number obj_value,
                             Number inf_pr, Number inf_du,
                             Number mu, Number d_norm,
                             Number regularization_size,
                             Number alpha_du, Number alpha_pr,
                             Index ls_trials,
                             const IpoptData* ip_data,
                             IpoptCalculatedQuantities* ip_cq)
        throw ()
      {
        return true;
      }

      virtual Index
      get_number_of_nonlinear_variables () throw ()
      {
        //FIXME: implement this.
        return -1;
      }

      virtual bool
      get_list_of_nonlinear_variables (Index num_nonlin_vars,
                                       Index* pos_nonlin_vars)
        throw ()
      {
        //FIXME: implement this.
        return false;
      }

      IpoptSolver& solver_;
    };
  }; // end of namespace detail

  using namespace detail;

  IpoptSolver::IpoptSolver (const problem_t& pb) throw ()
    : parent_t (pb),
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

  void
  IpoptSolver::solve () throw ()
  {
    ApplicationReturnStatus status = app_->Initialize ("");
    if (status != Solve_Succeeded)
      //FIXME: put a more precise error message.
      result_ = SolverError ("Ipopt application failed to initialize.");
    app_->OptimizeTNLP (nlp_);
  }

  Ipopt::SmartPtr<Ipopt::IpoptApplication>
  IpoptSolver::getIpoptApplication () throw ()
  {
    return app_;
  }

} // end of namespace optimization
