// Copyright (C) 2015 by Benjamin Chrétien, CNRS-UM LIRMM.
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

#ifndef ROBOPTIM_CORE_RESULT_ANALYZER_HXX
# define ROBOPTIM_CORE_RESULT_ANALYZER_HXX

# include <vector>
# include <limits>

# include <Eigen/OrderingMethods>

# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>
# include <roboptim/core/terminal-color.hh>

namespace roboptim
{
  template <typename T>
  ResultAnalyzer<T>::ResultAnalyzer (const problem_t& pb,
                                     const optimResult_t& res,
                                     value_type eps)
  : pb_ (pb),
    res_ (res),
    activeJac_ (),
    jac_ (),
    activeCstrIndices_ (),
    ineqIndices_ (),
    eps_ (eps)
  {
  }

  template <typename T>
  ResultAnalyzer<T>::~ResultAnalyzer ()
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::hasLambda () const
  {
    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();

    return (res_.lambda.size () == n + m + 1);
  }

  template <typename T>
  bool ResultAnalyzer<T>::QueryData::operator_bool () const
  {
    return isValid ();
  }

  template <typename T>
  ResultAnalyzer<T>::LICQData::LICQData ()
  : rank (0),
    max_rank (0)
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::LICQData::isValid () const
  {
    return rank == max_rank;
  }

  template <typename T>
  std::ostream& ResultAnalyzer<T>::LICQData::print (std::ostream& o) const
  {
    o << fg::bold << "LICQ conditions: " << fg::reset;
    if (isValid ()) o << fg::green << "satisfied" << fg::reset;
    else o << fg::red << "not satisfied" << fg::reset;

    o << incindent << iendl << "rank: " << rank
      << iendl << "max_rank: " << max_rank << decindent;

    return o;
  }

  template <typename T>
  ResultAnalyzer<T>::KKTData::KKTData ()
  : grad_L (),
    lambda (),
    violation (std::numeric_limits<value_type>::infinity ()),
    complementary_slackness (std::numeric_limits<value_type>::infinity ()),
    dual_feasible (false),
    eps (std::numeric_limits<value_type>::epsilon ())
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::KKTData::isValid () const
  {
    bool stationarity = (grad_L.size () > 0) && (grad_L.norm () < eps);
    bool primal_feasibility = (violation < eps);

    return stationarity && primal_feasibility && dual_feasible &&
           (complementary_slackness < eps);
  }

  template <typename T>
  std::ostream& ResultAnalyzer<T>::KKTData::print (std::ostream& o) const
  {
    o << fg::bold << "KKT conditions: " << fg::reset;
    if (isValid ()) o << fg::green << "satisfied" << fg::reset;
    else o << fg::red << "not satisfied" << fg::reset;

    o << incindent;

    if (grad_L.size () == 0)
      o << iendl << "∇L(x*,λ*) could not be computed";
    else
      o << iendl << "∇L(x*,λ*) = " << grad_L;

    if (lambda.size () == 0)
      o << iendl << "λ was not provided";
    else
      o << iendl << "λ = " << lambda;

    o << iendl << "constraint violation = " << violation;
    o << iendl << "complementary slackness = " << complementary_slackness;
    o << iendl << "dual feasible: " << (dual_feasible? "true":"false");

    o << decindent;

    return o;
  }

  template <typename T>
  ResultAnalyzer<T>::NullGradientData::NullGradientData ()
  : null_rows (0),
    constraint_indices ()
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::NullGradientData::isValid () const
  {
    return null_rows == 0;
  }

  template <typename T>
  std::ostream& ResultAnalyzer<T>::NullGradientData::print (std::ostream& o) const
  {
    o << fg::bold << "Null gradient condition: " << fg::reset;
    if (isValid ()) return o << fg::green << "satisfied" << fg::reset;
    else o << fg::red << "not satisfied" << fg::reset;

    o << incindent;
    for (typename std::map<const constraint_t, std::vector<size_type> >::const_iterator
         c  = constraint_indices.begin ();
         c != constraint_indices.end (); ++c)
    {
      o << iendl << "- Constraint: " << incindent
        << *(c->first) << decindent
        << iendl << "  Null gradient indices: " << c->second;
    }
    o << decindent;

    return o;
  }

  template <typename T>
  void ResultAnalyzer<T>::computeJacobian () const
  {
    if (jac_.cols () + jac_.rows () != 0)
      return;

    jac_ = pb_.jacobian (res_.x);
  }

  template <typename T>
  void ResultAnalyzer<T>::computeActiveJacobian () const
  {
    if (activeJac_.cols () + activeJac_.rows ()!= 0)
      return;

    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();

    const argument_t& x = res_.x;

    // 1) Argument bounds
    std::vector<size_type> activeBounds;
    for (size_type i = 0; i < n; ++i)
    {
      const interval_t& bounds = pb_.argumentBounds ()[static_cast<size_t> (i)];
      if (std::abs (x[i] - bounds.first) < eps_
          || std::abs (x[i] - bounds.second) < eps_)
        activeBounds.push_back (i);
    }

    // 2) Constraints
    std::vector<size_type> activeConstraints;
    size_type idx = 0;
    size_t cstr_idx = 0;

    for (typename problem_t::constraints_t::const_iterator
         cstr  = pb_.constraints ().begin ();
         cstr != pb_.constraints ().end (); ++cstr, ++cstr_idx)
    {
      const typename problem_t::intervals_t&
        bounds = pb_.boundsVector ()[cstr_idx];
      result_t val = (**cstr) (x);
      for (size_type j = 0; j < val.size (); ++j, ++idx)
      {
        size_t jj = static_cast<size_t> (j);
        if (std::abs (val[j] - bounds[jj].first) < eps_
            || std::abs (val[j] - bounds[jj].second) < eps_)
        {
          activeConstraints.push_back (idx);

          ConstraintIndex index;
          index.global = cstr_idx;
          index.local = j;
          index.active = static_cast<size_type> (activeCstrIndices_.size ());
          activeCstrIndices_.push_back (index);
        }
      }
    }

    size_type m_act = static_cast<size_type> (activeBounds.size ()
        + activeConstraints.size ());
    activeJac_.resize (m_act, n);
    activeJac_.setZero ();

    for (size_type i = 0; i < static_cast<size_type> (activeBounds.size ()); ++i)
    {
      // FIXME: sparse
      activeJac_ (i, activeBounds[static_cast<size_t> (i)]) = 1.;
    }

    computeJacobian ();

    for (size_t i = 0; i < activeConstraints.size (); ++i)
    {
      // Index in the global Jacobian of active constraints
      size_type idx = static_cast<size_type> (activeBounds.size () + i);
      // FIXME: sparse
      activeJac_.row (idx) = jac_.row (activeConstraints[i]);
    }
  }

  template <typename T>
  void ResultAnalyzer<T>::findIneqIndices () const
  {
    if (ineqIndices_.size () > 0)
      return;

    size_type n = pb_.function ().inputSize ();

    // Argument bounds
    size_type idx = 0;
    for (size_type i = 0; i < n; ++i, ++idx)
    {
      const interval_t& bounds = pb_.argumentBounds ()[static_cast<size_t> (i)];
      if (bounds.first != bounds.second) ineqIndices_.push_back (idx);
    }

    // Constraints bounds
    size_type cstr_idx = 0;
    for (size_t cstr_idx = 0; cstr_idx < pb_.constraints ().size (); ++cstr_idx)
    {
      const typename problem_t::intervals_t&
        bounds = pb_.boundsVector ()[cstr_idx];

      for (size_t j = 0; j < bounds.size (); ++j, ++idx)
      {
        if (bounds[j].first != bounds[j].second)
          ineqIndices_.push_back (idx);
      }
    }
  }

  template <>
  inline typename ResultAnalyzer<EigenMatrixSparse>::size_type
  ResultAnalyzer<EigenMatrixSparse>::computeRank (const jacobian_t& jac) const
  {
    Eigen::SparseQR<jacobian_t, Eigen::COLAMDOrdering<int> > qr (jac);
    return static_cast<size_type> (qr.rank ());
  }

  template <typename T>
  typename ResultAnalyzer<T>::size_type
  ResultAnalyzer<T>::computeRank (const jacobian_t& jac) const
  {
    Eigen::FullPivLU<jacobian_t> lu (jac);
    return static_cast<size_type> (lu.rank ());
  }

  template <typename T>
  typename ResultAnalyzer<T>::LICQData
  ResultAnalyzer<T>::checkLICQ () const
  {
    computeActiveJacobian ();

    LICQData licq;

    licq.max_rank = static_cast<size_type> (std::min (activeJac_.rows (),
                                                      activeJac_.cols ()));
    licq.rank = computeRank (activeJac_);

    return licq;
  }

  template <typename T>
  typename ResultAnalyzer<T>::KKTData
  ResultAnalyzer<T>::checkKKT () const
  {
    KKTData kkt;

    if (!hasLambda ())
      return kkt;

    computeActiveJacobian ();
    findIneqIndices ();

    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();
    const argument_t& x = res_.x;

    const differentiableFunction_t* diffObj
      = pb_.function ().template castInto<differentiableFunction_t> ();
    kkt.grad_L = diffObj->gradient (x);

    // Argument bounds
    kkt.grad_L += res_.lambda.segment (0, n);

    // Constraints
    kkt.grad_L += res_.lambda.segment (n, m).transpose () * jac_;

    // Store lambda
    kkt.lambda = res_.lambda;

    // Set epsilon
    kkt.eps = eps_;

    // Set constraint violation
    result_t violations = pb_.constraintsViolationVector (x);
    kkt.violation = violations.template lpNorm<Eigen::Infinity> ();

    // Complementary slackness and dual feasibility
    kkt.dual_feasible = true;
    kkt.complementary_slackness = 0.;
    for (size_t i = 0; i < ineqIndices_.size (); ++i)
    {
      size_type ii = ineqIndices_[i];
      kkt.complementary_slackness += kkt.lambda (ii) * violations[ii];
      value_type sgn = 1.0;
      // If lower bound constraint, i.e. 0 ≤ g
      if (violations[ii] < 0.) sgn = -1.0;

      // Valid if λ ≥ 0 for g ≤ 0
      if (kkt.dual_feasible && sgn * kkt.lambda (ii) < 0.)
      {
        kkt.dual_feasible = false;
      }
    }

    return kkt;
  }

  template <typename T>
  typename ResultAnalyzer<T>::NullGradientData
  ResultAnalyzer<T>::checkNullGradient () const
  {
    computeActiveJacobian ();

    NullGradientData null_grad;

    for (typename std::vector<ConstraintIndex>::const_iterator
         ci  = activeCstrIndices_.begin ();
         ci != activeCstrIndices_.end (); ++ci)
    {
      size_t g_idx = ci->global;
      size_type l_idx = ci->local;
      size_type as_idx = ci->active;

      if (activeJac_.row (as_idx).norm () < eps_)
      {
        null_grad.null_rows++;
        null_grad.constraint_indices[pb_.constraints ()[g_idx]].push_back (l_idx);
      }
    }

    return null_grad;
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_ANALYZER_HXX
