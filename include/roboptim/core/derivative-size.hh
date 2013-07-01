// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_DERIVATIVE_SIZE_HH
# define ROBOPTIM_CORE_DERIVATIVE_SIZE_HH
# include <roboptim/core/fwd.hh>

namespace roboptim
{
  template <typename T>
  struct derivativeSize< GenericFunction<T> >
  {
    static const unsigned int value = 0;
  };

  template <typename T>
  struct derivativeSize< GenericDifferentiableFunction<T> >
  {
    static const unsigned int value = 1;
  };

  template <typename T>
    struct derivativeSize< GenericTwiceDifferentiableFunction<T> >
  {
    static const unsigned int value = 2;
  };

  template <unsigned N>
  struct derivativeSize<NTimesDerivableFunction<N> >
  {
    static const unsigned int value = N;
  };
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DERIVATIVE_SIZE_HH
