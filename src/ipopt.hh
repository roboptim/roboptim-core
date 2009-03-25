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
 * \file ipopt.hh
 *
 * \brief Implementation of the Ipopt class.
 */

#ifndef OPTIMIZATION_IPOPT_HH
# define OPTIMIZATION_IPOPT_HH
# include <cassert>
# include <IpIpoptApplication.hpp>
# include <IpTNLP.hpp>

# include <solver.hh>

namespace optimization
{
  using namespace Ipopt;

  namespace
  {
    template <typename F>
    struct MyTNLP : public TNLP
    {
      MyTNLP (const IpoptSolver<F>& solver)
        : solver_ (solver)
      {
      }

      virtual bool
      get_nlp_info (Index& n, Index& m, Index& nnz_jac_g,
                    Index& nnz_h_lag, TNLP::IndexStyleEnum& index_style)
      {
        n = IpoptSolver<F>::arity;
        m = 0;
        nnz_jac_g = 0;
        nnz_h_lag = 0;
        index_style = TNLP::C_STYLE;
        return true;
      }
      virtual bool
      get_bounds_info (Index n, Number* x_l, Number* x_u,
                       Index m, Number* g_l, Number* g_u)
      {
        return true;
      }

      virtual bool
      get_starting_point(Index n, bool init_x, Number* x,
                         bool init_z, Number* z_L, Number* z_U,
                         Index m, bool init_lambda,
                         Number* lambda)
      {
        return true;
      }

      virtual bool
      eval_f (Index n, const Number* x, bool new_x, Number& obj_value)
      {
        //FIXME: call the function for real.
        //obj_value = solver_.function_ (x[0]);
        return true;
      }

      virtual bool
      eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
      {
        return true;
      }

      virtual bool
      eval_g(Index n, const Number* x, bool new_x,
                        Index m, Number* g)
      {
        return true;
      }

      virtual bool
      eval_jac_g(Index n, const Number* x, bool new_x,
                 Index m, Index nele_jac, Index* iRow,
                 Index *jCol, Number* values)
      {
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
      }

      const IpoptSolver<F>& solver_;
    };
  };

  template <typename F>
  class IpoptSolver : public Solver<F>
  {
  public:
    friend class MyTNLP<F>;
    typedef Solver<F> parent_t;

    explicit IpoptSolver (const typename parent_t::function_t& fct) throw ()
      : parent_t (fct),
        nlp_ (new MyTNLP<F> (*this)),
        app_ (new IpoptApplication ())
    {
    }

    virtual ~IpoptSolver () throw ()
    {
    }

    virtual typename parent_t::result_t
    getMinimum () throw ()
    {
      ApplicationReturnStatus status = app_->Initialize ();
      if (status != Solve_Succeeded)
        return typename parent_t::result_t (SolverError ());

      status = app_->OptimizeTNLP (nlp_);
      if (status != Solve_Succeeded)
        return typename parent_t::result_t (SolverError ());

      return typename parent_t::result_t (0.);
    }

  private:
    SmartPtr<TNLP> nlp_;
    SmartPtr<IpoptApplication> app_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_IPOPT_HH
