// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HXX
# define ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HXX
# include <boost/format.hpp>

namespace roboptim
{
  template <typename T>
  VectorInterpolation<T>::VectorInterpolation
  (const vector_t& x, size_type outputSize) throw ()
    : roboptim::GenericDifferentiableFunction<T>
      (1, outputSize,
       (boost::format ("vectorInterpolation(%1%)")
	% x).str ()),
      x_ (x),
      dx_ (x.size ())
  {
    if (x.size () % outputSize != 0)
      throw std::runtime_error ("x and outputSize are not compatible");

    //FIXME: this should be configurable.
    for (unsigned i = 0; i < x.size () - 1; ++i)
      dx_[i] = (x[i] - x[i + 1]) / 2.;
  }

  template <typename T>
  VectorInterpolation<T>::~VectorInterpolation () throw ()
  {}

  template <typename T>
  void
  VectorInterpolation<T>::impl_compute
  (result_t& result, const argument_t& x)
    const throw ()
  {
    size_type before = static_cast<size_type> (std::floor (x[0]));
    size_type after = static_cast<size_type> (std::ceil (x[0]));

    // If we are outsize the domain of definition, return zero.
    if (before < 0)
      return;
    if (after > x_.size ())
      return;

    result = x_.segment (before * this->outputSize (), this->outputSize ());
    // If we are exactly on a data point, just return.
    if (before == after)
      return;

    // Otherwise interpolate.
    // f(x) = (1. - alpha) * x_before + alpha * x_after
    double alpha = static_cast<value_type> (after) - x[0];
    result *= 1. - alpha;
    result +=
      alpha * x_.segment (after * this->outputSize (), this->outputSize ());
  }

  template <typename T>
  void
  VectorInterpolation<T>::impl_gradient (gradient_t& gradient,
				 const argument_t& x,
				 size_type functionId)
    const throw ()
  {
    size_type before = static_cast<size_type> (std::floor (x[0]));
    size_type after = static_cast<size_type> (std::ceil (x[0]));

    // If we are outsize the domain of definition, return zero.
    if (before < 0)
      return;
    if (after > x_.size ())
      return;

    gradient.coeffRef (0) =
      dx_.coeffRef (before * this->outputSize () + functionId);

    // If we are exactly on a data point, just return.
    if (before == after)
      return;

    // Otherwise interpolate.
    // f(x) = (1. - alpha) * x_before + alpha * x_after
    double alpha = static_cast<value_type> (after) - x[0];
    gradient.coeffRef (0) *= 1. - alpha;
    gradient.coeffRef (0) +=
      alpha * dx_.coeffRef (after * this->outputSize () + functionId);
  }

  template <typename T>
  void
  VectorInterpolation<T>::impl_jacobian (jacobian_t& jacobian,
					   const argument_t& x)
    const throw ()
  {
    size_type before = static_cast<size_type> (std::floor (x[0]));
    size_type after = static_cast<size_type> (std::ceil (x[0]));

    // If we are outsize the domain of definition, return zero.
    if (before < 0)
      return;
    if (after > x_.size ())
      return;

    for (size_type i = 0; i < this->outputSize (); ++i)
      jacobian.coeffRef (0, i) =
	dx_.coeffRef (before * this->outputSize () + i);

    // If we are exactly on a data point, just return.
    if (before == after)
      return;

    // Otherwise interpolate.
    // f(x) = (1. - alpha) * x_before + alpha * x_after
    double alpha = static_cast<value_type> (after) - x[0];
    jacobian *= 1. - alpha;

    for (size_type i = 0; i < this->outputSize (); ++i)
      jacobian.coeffRef (0, i) +=
	alpha * dx_.coeffRef (after * this->outputSize () + i);

  }
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HXX
