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

#ifndef ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX
# define ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX

namespace roboptim
{
  template <typename FdgPolicy>
  FiniteDifferenceGradient<FdgPolicy>::FiniteDifferenceGradient
  (const Function& adaptee, value_type epsilon)
    throw ()
    : DerivableFunction (adaptee.inputSize (), adaptee.outputSize ()),
      FdgPolicy (),
      adaptee_ (adaptee),
      epsilon_ (epsilon)
  {
    // Avoid meaningless values for epsilon such as 0 or NaN.
    assert (epsilon != 0. && epsilon == epsilon);
  }

  template <typename FdgPolicy>
  FiniteDifferenceGradient<FdgPolicy>::~FiniteDifferenceGradient () throw ()
  {
  }

  template <typename FdgPolicy>
  void
  FiniteDifferenceGradient<FdgPolicy>::impl_compute
  (result_t& result, const argument_t& argument) const throw ()
  {
    adaptee_ (result, argument);
  }

  template <typename FdgPolicy>
  void
  FiniteDifferenceGradient<FdgPolicy>::impl_gradient
  (gradient_t& gradient,
   const argument_t& argument,
   size_type idFunction) const throw ()
  {
    this->computeGradient (adaptee_, epsilon_, gradient, argument, idFunction);
  }

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX
