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

#ifndef ROBOPTIM_CORE_TWICE_DIFFERENTIABLE_FUNCTION_HXX
# define ROBOPTIM_CORE_TWICE_DIFFERENTIABLE_FUNCTION_HXX
# include <boost/algorithm/string/replace.hpp>

# include <roboptim/core/indent.hh>
# include <roboptim/core/util.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  template <typename T>
  GenericTwiceDifferentiableFunction<T>::GenericTwiceDifferentiableFunction
  (size_type inputSize,
   size_type outputSize,
   std::string name)
    : GenericDifferentiableFunction<T> (inputSize, outputSize, name)
  {
  }

  template <typename T>
  std::ostream&
  GenericTwiceDifferentiableFunction<T>::print (std::ostream& o) const
  {
    if (this->getName ().empty ())
      return o << "Twice differentiable function";

    std::stringstream ss;
    ss << std::endl;
    char fill = o.fill (' ');
    ss << std::setw ((int)indent (o))
       << ""
       << std::setfill (fill);
    std::string name = this->getName ();
    boost::algorithm::replace_all (name, "\n", ss.str ());

    return o << name << " (twice differentiable function)";
  }

// Explicit template instantiations for dense and sparse matrices.
# ifdef ROBOPTIM_PRECOMPILED_DENSE_SPARSE
  ROBOPTIM_ALLOW_ATTRIBUTES_ON
  extern template class ROBOPTIM_CORE_DLLAPI GenericTwiceDifferentiableFunction<EigenMatrixDense>;
  extern template class ROBOPTIM_CORE_DLLAPI GenericTwiceDifferentiableFunction<EigenMatrixSparse>;
  ROBOPTIM_ALLOW_ATTRIBUTES_OFF
# endif

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_TWICE_DIFFERENTIABLE_FUNCTION_HXX
