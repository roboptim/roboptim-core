// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
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

#ifndef ROBOPTIM_CORE_SCALING_HELPER_HXX
# define ROBOPTIM_CORE_SCALING_HELPER_HXX

# include <vector>
# include <stdexcept>

# include <roboptim/core/util.hh>
# include <roboptim/core/differentiable-function.hh>

namespace roboptim
{
  template <typename T>
  ScalingHelper<T>::ScalingHelper (const problem_t& pb, const interval_t& gradRange)
  : pb_ (pb),
    gradRange_ (gradRange),
    scalingFunc_ ()
  {
    if (gradRange_.first > gradRange_.second
        || gradRange_.first < 0. || gradRange_.second < 0.)
      throw std::runtime_error ("invalid absolute gradient ranges");

    scalingFunc_.resize (pb_.constraints ().size ());
    size_t c_idx = 0;
    for (typename std::vector<scalingFunc_t>::iterator
         p  = scalingFunc_.begin ();
         p != scalingFunc_.end (); ++p)
    {
      p->first = pb_.constraints ()[c_idx++];
      p->second.resize (static_cast<size_t> (p->first->outputSize ()), 1.);
    }
  }

  template <typename T>
  ScalingHelper<T>::~ScalingHelper ()
  {
  }

  template <typename T>
  void ScalingHelper<T>::computeScaling (const std::vector<argument_t>& x)
  {
    typedef GenericDifferentiableFunction<T> differentiableFunction_t;
    typedef typename differentiableFunction_t::jacobian_t jacobian_t;

    std::vector<interval_t> bounds;
    jacobian_t jac;
    size_t c_idx = 0;

    // For each constraint
    for (typename constraints_t::const_iterator
         c  = pb_.constraints ().begin ();
         c != pb_.constraints ().end (); ++c, ++c_idx)
    {
      // If the constraints is differentiable
      if ((*c)->template asType<differentiableFunction_t> ())
      {
        const differentiableFunction_t*
          df = (*c)->template castInto<differentiableFunction_t> ();
        jac.resize (df->outputSize (), df->inputSize ());

        for (typename std::vector<argument_t>::const_iterator
             xi = x.begin (); xi != x.end (); ++xi)
        {
          jac.setZero ();
          df->jacobian (jac, *xi);

          // For each output dimension
          for (size_type i = 0; i < df->outputSize (); ++i)
          {
            // Get the maximum amplitude of the gradient
            value_type maxGrad = maxRow (jac, i);
            value_type& curScaling
              = scalingFunc_[c_idx].second[static_cast<size_t> (i)];

            // Gradient values too high
            if (maxGrad > gradRange_.second)
            {
               value_type scaling = gradRange_.second / maxGrad;
               if (scaling < curScaling)
               {
                 curScaling = scaling;
               }
            }
            // Gradient values too low
            else if (maxGrad < gradRange_.first && curScaling >= 1.)
            {
               value_type scaling = gradRange_.first / maxGrad;
               if (scaling > curScaling)
               {
                 curScaling = scaling;
               }
            }
          }
        }
      }
    }
  }

  template <typename T>
  std::ostream& ScalingHelper<T>::print (std::ostream& o) const
  {
    size_t c_idx = 0;

    if (scalingFunc_.empty ())
      return o << "No suggested scaling parameters";

    o << "Suggested scaling parameters:" << incindent;
    for (typename std::vector<scalingFunc_t>::const_iterator
         c  = scalingFunc_.begin ();
         c != scalingFunc_.end (); ++c, ++c_idx)
    {
      o << iendl << "Constraint " << c_idx << ":" << incindent
        << iendl << *(c->first)
        << iendl << "Scaling: " << c->second
        << decindent;
    }
    o << decindent;

    return o;
  }

  template <>
  inline ScalingHelper<EigenMatrixSparse>::value_type
  ScalingHelper<EigenMatrixSparse>::maxRow
  (const jacobian_t& jac, size_type i) const
  {
    typedef DifferentiableFunction::vector_t denseVector_t;

    // Note: lpNorm is not available for sparse Eigen::Block
    denseVector_t grad (jac.row (i));
    return grad.lpNorm<Eigen::Infinity> ();
  }

  template <typename T>
  typename ScalingHelper<T>::value_type
  ScalingHelper<T>::maxRow (const jacobian_t& jac, size_type i) const
  {
    return jac.row (i).template lpNorm<Eigen::Infinity> ();
  }

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const ScalingHelper<T>& sh)
  {
    return sh.print (o);
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  extern template class ROBOPTIM_CORE_DLLAPI
    ScalingHelper<EigenMatrixDense>;
  extern template class ROBOPTIM_CORE_DLLAPI
    ScalingHelper<EigenMatrixSparse>;
# endif //! ROBOPTIM_PRECOMPILED_DENSE_SPARSE

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SCALING_HELPER_HXX
