// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
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


/**
 * \brief Implementation of the CFSQP module.
 */
#include <limits>
#include <cfsqpusr.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/variant/apply_visitor.hpp>

#include "roboptim-core/cfsqp.hh"
#include "roboptim-core/function.hh"
#include "roboptim-core/indent.hh"
#include "roboptim-core/util.hh"

namespace roboptim
{
  typedef void (*fct_t) (int, int, double*, double*, void*);
  typedef void (*grad_t) (int, int, double*, double*, fct_t, void*);

  namespace detail
  {
    /// \internal
    /// CFSQP objective function.
    void obj (int nparam, int j , double* x, double* fj, void* cd)
    {
      assert (cd);
      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      Function::vector_t x_ (nparam);
      array_to_vector (x_, x);
      *fj = solver->problem ().function () (x_)[0];
    }


    /// \internal
    /// Evaluate an inequality defined from lower/upper bounds.
    double
    evaluate_inequality (double x, bool is_lower, double l, double u)
    {
      if (is_lower)
        // g(x) >= b, -g(x) + b <= 0
        return -x + l;
      else
        // g(x) <= b, g(x) - b <= 0
        return x - u;
    }

    /// \internal
    /// CFSQP constraints function.
    void constr (int nparam, int j,
                 double* x, double* gj, void* cd)
    {
      assert (cd && !!gj && !!x && nparam >= 0 && j > 0 && j <= nparam);
      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      Function::vector_t x_ (nparam);
      array_to_vector (x_, x);

      // Decrement j to have C style indexation (0...size - 1).
      j--;

      // Constraint index in the generic representation.
      int j_ = solver->cfsqpConstraints ()[j].first;

      if (0 <= j && j < solver->nineqn ())
        {
          const DerivableFunction& f =
            *boost::get<const DerivableFunction*>
            (solver->problem ().constraints ()[j_]);
          Function::vector_t res = f (x_);
          *gj = evaluate_inequality
            (res (0),
             solver->cfsqpConstraints ()[j].second,
             solver->problem ().bounds ()[j_].first,
             solver->problem ().bounds ()[j_].second);
          return;
        }

      if (solver->nineqn () <= j && j < solver->nineq () - solver->nineqn ())
        {
          const LinearFunction& f =
            *boost::get<const LinearFunction*>
            (solver->problem ().constraints ()[j_]);
          Function::vector_t res = f (x_);
          *gj = evaluate_inequality
            (res (0),
             solver->cfsqpConstraints ()[j].second,
             solver->problem ().bounds ()[j_].first,
             solver->problem ().bounds ()[j_].second);
          return;
        }

      j -= solver->nineq ();
      assert (j >= 0);
      if (0 <= j && j < solver->neqn ())
        {
          const DerivableFunction& f =
            *boost::get<const DerivableFunction*>
            (solver->problem ().constraints ()[j_]);
          Function::vector_t res = f (x_);
          *gj = res (0) - solver->problem ().bounds ()[j_].first;
          return;
        }

      if (solver->neqn () <= j && j < solver->neq () - solver->neqn ())
        {
          const LinearFunction& f =
            *boost::get<const LinearFunction*>
            (solver->problem ().constraints ()[j_]);
          Function::vector_t res = f (x_);
          *gj = res (0) - solver->problem ().bounds ()[j_].first;
          return;
        }
      assert (0);
    }

    /// \internal
    /// CFSQP objective function gradient.
    void gradob (int nparam, int j,
                 double* x, double* gradf, fct_t dummy, void* cd)
    {
      assert (nparam >= 0 && j == 1 && !!x && !!gradf && !!cd);

      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      Function::vector_t x_ (nparam);
      array_to_vector (x_, x);
      DerivableFunction::gradient_t grad =
        solver->problem ().function ().gradient (x_, 0);
      vector_to_array (gradf, grad);
    }

    /// \internal
    /// CFSQP constraints function gradient.
    void gradcn (int nparam, int j,
                 double* x, double* gradgj, fct_t dummy, void* cd)
    {
      assert (nparam >= 0 && j > 0 && j <= nparam && !!x && !!gradgj && !!cd);

      CFSQPSolver* solver = static_cast<CFSQPSolver*> (cd);

      Function::vector_t x_ (nparam);
      array_to_vector (x_, x);

      Function::vector_t grad (nparam);

      // Decrement j to have C style indexation (0...size - 1).
      j--;
      // Constraint index in the generic representation.
      int j_ = solver->cfsqpConstraints ()[j].first;

      if (solver->problem ().constraints ()[j_].which () == 0)
        {
          const DerivableFunction& f =
            *boost::get<const DerivableFunction*>
            (solver->problem ().constraints ()[j_]);
          grad = f.gradient (x_, 0);
        }
      else
        {
          const LinearFunction& f =
            *boost::get<const LinearFunction*>
            (solver->problem ().constraints ()[j_]);
          grad = f.gradient (x_, 0);
        }
      vector_to_array (gradgj, grad);
    }

  }

  CFSQPSolver::CFSQPSolver (const problem_t& pb, int iprint) throw ()
    : parent_t (pb),
      nineq_ (0),
      nineqn_ (0),
      neq_ (0),
      neqn_ (0),
      mode_ (100),
      iprint_ (iprint),
      miter_ (500),
      bigbnd_ (1e10),
      eps_ (1e-8),
      epseqn_ (1e-8),
      udelta_ (1e-8),
      cfsqpConstraints_ ()
  {
    // Add non-linear inequalities.
    for (unsigned i = 0; i < problem ().constraints ().size (); ++i)
      if (problem ().constraints ()[i].which () == 0)
        if (problem ().bounds ()[i].first != problem ().bounds ()[i].second)
          {
            if (problem ().bounds ()[i].first != Function::infinity ())
              cfsqpConstraints_.push_back (std::make_pair (i, true));
            if (problem ().bounds ()[i].second != Function::infinity ())
              cfsqpConstraints_.push_back (std::make_pair (i, false));
          }
    nineqn_ = cfsqpConstraints_.size ();

    // Add linear inequalities.
    for (unsigned i = 0; i < problem ().constraints ().size (); ++i)
      if (problem ().constraints ()[i].which () == 1)
        if (problem ().bounds ()[i].first != problem ().bounds ()[i].second)
          {
            if (problem ().bounds ()[i].first != Function::infinity ())
              cfsqpConstraints_.push_back (std::make_pair (i, true));
            if (problem ().bounds ()[i].second != Function::infinity ())
              cfsqpConstraints_.push_back (std::make_pair (i, false));
          }
    nineq_ = cfsqpConstraints_.size ();

    // Add non-linear equalities.
    for (unsigned i = 0; i < problem ().constraints ().size (); ++i)
      if (problem ().constraints ()[i].which () == 0)
        if (problem ().bounds ()[i].first == problem ().bounds ()[i].second)
          cfsqpConstraints_.push_back (std::make_pair (i, true));
    neqn_ = cfsqpConstraints_.size () - nineq_;

    // Add linear equalities.
    for (unsigned i = 0; i < problem ().constraints ().size (); ++i)
      if (problem ().constraints ()[i].which () == 1)
        if (problem ().bounds ()[i].first == problem ().bounds ()[i].second)
          cfsqpConstraints_.push_back (std::make_pair (i, true));
    neq_ = cfsqpConstraints_.size () - nineq_;

    assert (nineq_ >= nineqn_);
    assert (neq_ >= neqn_);
  }

  CFSQPSolver::~CFSQPSolver () throw ()
  {
  }

  void
  CFSQPSolver::initialize_bounds (double* bl, double* bu) const throw ()
  {
    typedef problem_t::bounds_t::const_iterator citer_t;

    Function::size_type i = 0;
    for (citer_t it = problem ().argBounds ().begin ();
         it != problem ().argBounds ().end (); ++it)
      {
        bl[i] = (*it).first, bu[i] = (*it).second;
        ++i;
      }
  }

  void
  CFSQPSolver::solve () throw ()
  {
    const int nparam = problem ().function ().n;
    const int nf = 1; //FIXME: only one objective function.
    const int nfsr = 0;

    const int ncsrl = 0;
    const int ncsrn = 0;
    int mesh_pts[1];
    int inform = 0;
    double bl[nparam];
    double bu[nparam];
    double x[nparam];
    double f[1];
    double g[nineq_ + neq_];
    double lambda[nparam + 1 + nineq_ + neq_];
    fct_t obj = detail::obj;
    fct_t constr = detail::constr;
    grad_t gradob = detail::gradob;
    grad_t gradcn = detail::gradcn;

    // Initialize bounds.
    initialize_bounds (bl, bu);

    // Copy starting point.
    if (!!problem ().startingPoint ())
      detail::vector_to_array (x, *problem ().startingPoint ());

    cfsqp (nparam, nf, nfsr, nineqn_, nineq_, neqn_, neq_, ncsrl,  ncsrn,
           mesh_pts, mode_,  iprint_, miter_, &inform, bigbnd_, eps_, epseqn_,
           udelta_, bl, bu, x, f, g, lambda,
           obj, constr, gradob, gradcn, this);

    Result res (nparam, 1);
    detail::array_to_vector (res.x, x);
    res.value (0) = f[0];
    res.lambda.resize (problem ().constraints ().size ());
    //FIXME: lambda?

    ResultWithWarnings resw (nparam, 1);
    detail::array_to_vector (resw.x, x);
    resw.value (0) = f[0];
    res.lambda.resize (problem ().constraints ().size ());
    //FIXME: lambda?

    switch (inform)
      {
        // Normal termination.
      case 0:
        result_ = res;
        break;

        // Warnings.
      case 3:
        resw.warnings.push_back (SolverWarning
                                 ("Max iteration has been reached."));
        result_ = resw;
        break;

      case 4:
        resw.warnings.push_back (SolverWarning
                                 ("Failed to find a new iterate."));
        result_ = resw;
        break;

      case 8:
        resw.warnings.push_back (SolverWarning
                                 ("New iterate equivalent to previous one."));
        result_ = resw;
        break;


        // Errors.
      case 1:
        result_ = SolverError ("Infeasible guess for linear constraints.");
        break;

      case 2:
        result_ =
          SolverError
          ("Infeasible guess for linear and non-linear constraints.");
        break;

      case 5:
        result_ =
          SolverError
          ("Failed to construct d0.");
        break;

      case 6:
        result_ =
          SolverError
          ("Failed to construct d1.");
        break;

      case 7:
        result_ =
          SolverError
          ("Input data are not consistent.");
        break;

      case 9:
        result_ =
          SolverError
          ("One penalty parameter has exceeded bigbng.");
        break;

      default:
        result_ = SolverError ("CFSQP has failed.");
      }
  }


  const std::vector<std::pair<int, bool> >&
  CFSQPSolver::cfsqpConstraints () const throw ()
  {
    return cfsqpConstraints_;
  }

  const int&
  CFSQPSolver::nineqn () const throw ()
  {
    return nineqn_;
  }

  const int&
  CFSQPSolver::nineq () const throw ()
  {
    return nineq_;
  }

  const int&
  CFSQPSolver::neqn () const throw ()
  {
    return neqn_;
  }

  const int&
  CFSQPSolver::neq () const throw ()
  {
    return neq_;
  }


  const int&
  CFSQPSolver::mode () const throw ()
  {
    return mode_;
  }

  int&
  CFSQPSolver::iprint () throw ()
  {
    reset ();
    return iprint_;
  }

  const int&
  CFSQPSolver::iprint () const throw ()
  {
    return iprint_;
  }


  int&
  CFSQPSolver::miter () throw ()
  {
    reset ();
    return miter_;
  }

  const int&
  CFSQPSolver::miter () const throw ()
  {
    return miter_;
  }

  double&
  CFSQPSolver::bigbnd () throw ()
  {
    reset ();
    return bigbnd_;
  }

  const double&
  CFSQPSolver::bigbnd () const throw ()
  {
    return bigbnd_;
  }

  double&
  CFSQPSolver::eps () throw ()
  {
    reset ();
    return eps_;
  }

  const double&
  CFSQPSolver::eps () const throw ()
  {
    return eps_;
  }

  double&
  CFSQPSolver::epseqn () throw ()
  {
    reset ();
    return epseqn_;
  }

  const double&
  CFSQPSolver::epseqn () const throw ()
  {
    return epseqn_;
  }

  double&
  CFSQPSolver::udelta () throw ()
  {
    reset ();
    return udelta_;
  }

  const double&
  CFSQPSolver::udelta () const throw ()
  {
    return udelta_;
  }

  std::ostream&
  CFSQPSolver::print (std::ostream& o) const throw ()
  {
    parent_t::print (o);

    o << iendl << "CFSQP specific variables: " << incindent << iendl
      << "Nineq: " << nineq () << iendl
      << "Nineqn: " << nineqn () << iendl
      << "Neq: " << neq () << iendl
      << "Neqn: " << neqn () << iendl
      << "Mode: " << mode () << iendl
      << "Iprint: " << iprint () << iendl
      << "Miter: " << miter () << iendl
      << "Bigbnd: " << bigbnd () << iendl
      << "Eps: " << eps () << iendl
      << "Epseqn: " << epseqn () << iendl
      << "Udelta: " << udelta () << iendl
      << "CFSQP constraints: " << cfsqpConstraints ();

    return o;
  }

} // end of namespace roboptim
