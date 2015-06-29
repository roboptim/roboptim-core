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

#ifndef ROBOPTIM_CORE_QUADRATIC_FUNCTION_HXX
# define ROBOPTIM_CORE_QUADRATIC_FUNCTION_HXX

namespace roboptim
{
  template <typename T>
  GenericQuadraticFunction<T>::GenericQuadraticFunction (size_type inputSize,
							 size_type outputSize,
							 std::string name)
    : GenericTwiceDifferentiableFunction<T> (inputSize, outputSize, name)
  {}

  template <typename T>
  std::ostream&
  GenericQuadraticFunction<T>::print (std::ostream& o) const
  {
    if (this->getName ().empty ())
      return o << "Quadratic function";
    else
      return o << this->getName () << " (quadratic function)";
  }

  template <typename T>
  const unsigned long GenericQuadraticFunction<T>::flag_ = ROBOPTIM_IS_QUADRATIC|GenericQuadraticFunction<T>::parent_t::flag_;
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HXX
