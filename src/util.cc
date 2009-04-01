// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file src/util.cc
 *
 * \brief Useful procedures.
 */

#include <boost/numeric/ublas/matrix.hpp>

#include "util.hh"

namespace optimization
{
  namespace detail
  {
    namespace ublas = boost::numeric::ublas;

    void
    vector_to_array (Function::value_type* dst, const Function::vector_t& src)
    {
      memcpy (dst, &src[0], src.size () * sizeof (Function::value_type));

      for (std::size_t i = 0; i < src.size (); ++i)
        assert (dst[i] = src[i]);
    }

    void
    array_to_vector (Function::vector_t& dst, const Function::value_type* src)
    {
      memcpy (&dst[0], src, dst.size () * sizeof (Function::value_type));

      for (std::size_t i = 0; i < dst.size (); ++i)
        assert (dst[i] = src[i]);
    }

    void
    jacobian_from_gradients (Function::matrix_t& jac,
                             const Problem::constraints_t& c,
                             const Function::vector_t& x)
    {
      for (unsigned i = 0; i < jac.size1 (); ++i)
        {
          Function::gradient_t grad = c[i]->gradient (x);
          for (unsigned j = 0; j < jac.size2 (); ++j)
            jac (i, j) = (*grad)[j];
        }
    }
  }; // end of namespace detail.
}; // end of namespace optimization.
