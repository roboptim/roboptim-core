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

#ifndef ROBOPTIM_CORE_LINEAR_FUNCTION_HXX
# define ROBOPTIM_CORE_LINEAR_FUNCTION_HXX
# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>

namespace roboptim
{
  template <typename T>
  GenericLinearFunction<T>::GenericLinearFunction (size_type inputSize,
						   size_type outputSize,
						   std::string name) throw ()
    : GenericQuadraticFunction<T> (inputSize, outputSize, name)
  {}

  template <typename T>
  void
  GenericLinearFunction<T>::impl_hessian (hessian_t& hessian,
					  const argument_t&,
					  size_type) const throw ()
  {
    setZero (hessian);
  }

  template <typename T>
  std::ostream&
  GenericLinearFunction<T>::print (std::ostream& o) const throw ()
  {
    if (this->getName ().empty ())
      return o << "Linear function";
    else
      return o << this->getName () << " (linear function)";
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_LINEAR_FUNCTION_HXX
