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
} // end of namespace roboptim.
