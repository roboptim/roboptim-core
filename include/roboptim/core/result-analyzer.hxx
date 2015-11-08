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

namespace roboptim
{
  template <typename T>
  ResultAnalyzer<T>::ResultAnalyzer (const problem_t& pb,
                                     const optimResult_t& res)
  : pb_ (pb),
    res_ (res),
    activeJac_ (),
    jac_ (),
    eps_ (1e-6)
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
  ResultAnalyzer<T>::QueryData::operator bool () const
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
  ResultAnalyzer<T>::KKTData::KKTData ()
  : grad_L (),
    eps (std::numeric_limits<value_type>::epsilon ())
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::KKTData::isValid () const
  {
    // TODO
    return grad_L.size () > 0 && grad_L.norm () < eps;
  }

  template <typename T>
  ResultAnalyzer<T>::NullGradientData::NullGradientData ()
  : null_rows (0)
  {
  }

  template <typename T>
  bool ResultAnalyzer<T>::NullGradientData::isValid () const
  {
    return null_rows == 0;
  }

  template <typename T>
  void ResultAnalyzer<T>::computeJacobian () const
  {
    if (jac_.size () != 0)
      return;

    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();
    const argument_t& x = res_.x;

    jac_ = jacobian_t (m, n);
    jac_.setZero ();

    size_type row_iter = 0;
    for (typename problem_t::constraints_t::const_iterator
        cstr  = pb_.constraints ().begin ();
        cstr != pb_.constraints ().end (); ++cstr)
    {
      if ((*cstr)->template asType<differentiableFunction_t> ())
      {
        const differentiableFunction_t*
          diffCstr = (*cstr)->template castInto<differentiableFunction_t> ();
        diffCstr->jacobian
          (jac_.block (row_iter, 0,
                       (*cstr)->outputSize (), n), x);
      }
      row_iter += (*cstr)->outputSize ();
    }
  }

  template <typename T>
  void ResultAnalyzer<T>::computeActiveJacobian () const
  {
    if (activeJac_.size () != 0)
      return;

    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();

    bool needs_compute = ! hasLambda ();
    const argument_t& x = res_.x;

    // 1) Argument bounds
    std::vector<size_type> activeBounds;
    for (size_type i = 0; i < n; ++i)
    {
      if (!needs_compute)
      {
        if (res_.lambda[i] != 0)
          activeBounds.push_back (i);
      }
      else
      {
        const interval_t& bounds = pb_.argumentBounds ()[static_cast<size_t> (i)];
        if (std::abs (x[i] - bounds.first) < eps_
            || std::abs (x[i] - bounds.second) < eps_)
          activeBounds.push_back (i);
      }
    }

    // 2) Constraints
    std::vector<size_type> activeConstraints;
    if (!needs_compute)
    {
      for (int i = 0; i < m; ++i)
        // TODO: epsilon threshold?
        if (res_.lambda[n + i] != 0)
          activeConstraints.push_back (static_cast<size_type> (n + i));
    }
    else
    {
      size_type idx = 0;
      size_t cstr_idx = 0;

      for (typename problem_t::constraints_t::const_iterator
           cstr  = pb_.constraints ().begin ();
           cstr != pb_.constraints ().end (); ++cstr)
      {
        const typename problem_t::intervals_t&
          bounds = pb_.boundsVector ()[cstr_idx];
        result_t val = (**cstr) (x);
        for (size_type j = 0; j < val.size (); ++j)
        {
          size_t jj = static_cast<size_t> (j);
          if (std::abs (val[j] - bounds[jj].first) < eps_
              || std::abs (val[j] - bounds[jj].second) < eps_)
            activeConstraints.push_back (idx);
          idx++;
        }
        cstr_idx++;
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
      size_type idx = static_cast<size_type> (activeBounds.size () + i);
      // FIXME: sparse
      activeJac_.row (idx) = jac_.row (activeConstraints[i]);
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
    computeActiveJacobian ();

    KKTData kkt;

    if (!hasLambda ())
      return kkt;

    size_type n = pb_.function ().inputSize ();
    size_type m = pb_.constraintsOutputSize ();
    const argument_t& x = res_.x;

    const differentiableFunction_t* diffObj
      = pb_.function ().template castInto<differentiableFunction_t> ();
    kkt.grad_L = diffObj->gradient (x);

    // Argument bounds
    kkt.grad_L += res_.lambda.segment (0, n);

    // Constraints
    kkt.grad_L += res_.lambda.segment (n, m) * jac_.transpose ();

    // TODO

    return kkt;
  }

  template <typename T>
  typename ResultAnalyzer<T>::NullGradientData
  ResultAnalyzer<T>::checkNullGradient () const
  {
    computeActiveJacobian ();

    NullGradientData null_grad;

    for (int i = 0; i < activeJac_.rows (); ++i)
    {
      if (activeJac_.row (i).norm () < eps_)
        null_grad.null_rows++;
    }

    return null_grad;
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_ANALYZER_HXX
