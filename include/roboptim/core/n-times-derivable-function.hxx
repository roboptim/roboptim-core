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

#ifndef ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX
# define ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX

namespace roboptim
{
  template <unsigned dorder>
  NTimesDerivableFunction<dorder>::~NTimesDerivableFunction () throw ()
  {
  }

  template <unsigned dorder>
  std::ostream&
  NTimesDerivableFunction<dorder>::print (std::ostream& o) const
    throw ()
  {
    o << derivabilityOrder << "-time differentiable function.";
    return o;
  }

}
#endif //! ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX
