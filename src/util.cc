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

#include "debug.hh"

#include <cstring>

#include <algorithm>
#include "roboptim/core/util.hh"

namespace roboptim
{
  namespace detail
  {
    void
    vector_to_array (Function::value_type* dst, const Function::vector_t& src)
    {
      if (src.size () == 0)
	return;
      Eigen::Map<Eigen::VectorXd>(dst, src.size ()) = src;

      // NaN != NaN, handle this case.
      for (Function::size_type i = 0; i < src.size (); ++i)
	if (src[i] != src[i])
	  assert (dst[i] != dst[i]);
	else
	  assert (dst[i] == src[i]);
    }

    void
    array_to_vector (Function::vector_t& dst, const Function::value_type* src)
    {
      if (dst.size () == 0)
	return;
      dst = Eigen::Map<const Eigen::VectorXd>(src, dst.size ());

      // NaN != NaN, handle this case.
      for (Function::size_type i = 0; i < dst.size (); ++i)
	if (src[i] != src[i])
	  assert (dst[i] != dst[i]);
	else
	  assert (dst[i] == src[i]);
    }
  } // end of namespace detail.

  GenericFunctionTraits<EigenMatrixDense>::matrix_t sparse_to_dense
  (const GenericFunctionTraits<EigenMatrixSparse>::matrix_t& m)
  {
    GenericFunctionTraits<EigenMatrixDense>::matrix_t
      dense(m.rows(), m.cols());
    for (int k = 0; k < m.outerSize(); ++k)
      for (GenericFunctionTraits<EigenMatrixSparse>::matrix_t::InnerIterator
	     it(m,k); it; ++it)
	{
	  dense(it.row(), it.col()) = it.value();
	}
    return dense;
  }

  GenericFunctionTraits<EigenMatrixDense>::vector_t sparse_to_dense
  (const GenericFunctionTraits<EigenMatrixSparse>::gradient_t& v)
  {
    GenericFunctionTraits<EigenMatrixDense>::vector_t dense(v.size());
    for (GenericFunctionTraits<EigenMatrixSparse>::gradient_t::InnerIterator
           it(v); it; ++it)
      {
	dense(it.index()) = it.value();
      }
    return dense;
  }
} // end of namespace roboptim.
