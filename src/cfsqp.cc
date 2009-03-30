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
 * \file src/cfsqp.cc
 *
 * \brief Implementation of the CFSQP module.
 */
#include <limits>
#include <cfsqpusr.h>

#include "cfsqp.hh"

namespace optimization
{
  typedef void (*fct_t) (int, int, double*, double*, void*);
  typedef void (*grad_t) (int, int, double*, double*, fct_t, void*);

  namespace detail
  {
    //FIXME: BEGIN should not duplicate that.
//     static void
//     vector_to_array (Solver::value_type* dst, const Solver::array_t& src)
//     {
//       memcpy (dst, &src[0], src.size () * sizeof (Solver::value_type));

//       for (std::size_t i = 0; i < src.size (); ++i)
//         assert (dst[i] = src[i]);
//     }

    static void
    array_to_vector (Solver::array_t& dst, const Solver::value_type* src)
    {
      memcpy (&dst[0], src, dst.size () * sizeof (Solver::value_type));

      for (std::size_t i = 0; i < dst.size (); ++i)
        assert (dst[i] = src[i]);
    }
    //FIXME: END should not duplicate that.



    void obj (int nparam, int j , double* x, double* fj, void* cd)
    {
      assert (cd);
      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      CFSQPSolver::array_t x_ (nparam);
      array_to_vector (x_, x);
      *fj = solver->getFunction () (x_);
    }

    void constr (int nparam, int j,
                 double* x, double* gj, void* cd)
    {
      assert (cd);
      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      assert (solver->getConstraints ().size () - j == 0);

      CFSQPSolver::array_t x_ (nparam);
      array_to_vector (x_, x);

      *gj = solver->getConstraints () [j].function (x_);
    }

    void gradob (int nparam, int j,
                 double* x, double* gradf, fct_t dummy, void* cd)
    {
      assert (cd);
      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      if (solver->getGradient ().empty ())
        ::grobfd (nparam, j, x, gradf, dummy, cd);

      //FIXME: should use gradient here.
      ::grobfd (nparam, j, x, gradf, dummy, cd);
    }

    void gradcn (int nparam, int j,
                 double* x, double* gradgj, fct_t dummy, void* cd)
    {
      assert (cd);
      //CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      //FIXME: if gradient of constraints available, use it else...
      //if (!solver.getGradient ())
      ::grcnfd (nparam, j, x, gradgj, dummy, cd);
    }

  }

  CFSQPSolver::CFSQPSolver (function_t fct,
                            size_type n,
                            gradient_t g,
                            hessian_t h,
                            jacobian_t j) throw ()
    : Solver (fct, n, g, h, j)
  {
  }

  CFSQPSolver::~CFSQPSolver () throw ()
  {
  }

  CFSQPSolver::result_t
  CFSQPSolver::getMinimum () throw ()
  {
    if (result_.which () != SOLVER_NO_SOLUTION)
      return result_;

    int nparam = getArity ();
    int nf = 1; //FIXME: only one objective function.
    int nfsr = 0;
    int nineqn = 0;
    int nineq = 0;
    int neqn = 0;
    int neq = 0;
    int ncsrl = 0;
    int ncsrn = 0;
    int mesh_pts;
    int mode = 100;
    int iprint = 2;
    int miter = 500;
    int inform = 0;
    double bignd = std::numeric_limits<value_type>::infinity ();
    double eps = 1e-8;
    double epseqn = 1e-8;
    double udelta = 1e-8;
    double bl[getArity ()];
    double bu[getArity ()];
    double x[getArity ()];
    double f = 0.;
    double g = 0.;
    double lambda[getArity ()];
    fct_t obj = detail::obj;
    fct_t constr = detail::constr;
    grad_t gradob = detail::gradob;
    grad_t gradcn = detail::gradcn;
    void* cd = this;

    // Initialize bounds.
    for (size_type i = 0; i < getArity (); ++i)
      {
        bl[i] = -std::numeric_limits<value_type>::infinity ();
        bu[i] = std::numeric_limits<value_type>::infinity ();
        lambda[i] = 1.; //FIXME: what's that?
      }

    // Copy starting point.
    if (start_)
      memcpy (x, &(*start_)[0], getArity () * sizeof (value_type));


    cfsqp (nparam, nf, nfsr, nineqn, nineq, neqn, neq, ncsrl,  ncsrn,
           &mesh_pts, mode,  iprint, miter, &inform, bignd, eps, epseqn,
           udelta, bl, bu, x, &f, &g, lambda,
           obj, constr, gradob, gradcn, cd);

    return result_;
  }

} // end of namespace optimization
