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
 * \brief Useful procedures (templated) implementation.
 */
#ifndef OPTIMIZATION_UTIL_HXX
# define OPTIMIZATION_UTIL_HXX
# include <liboptimization/function.hh>

namespace optimization
{
  namespace detail
  {
    template <typename T>
    void
    jacobian_from_gradients (DerivableFunction::matrix_t& jac,
                             const std::vector<const T*>& c,
                             const DerivableFunction::vector_t& x)
    {
      for (unsigned i = 0; i < jac.size1 (); ++i)
        {
          DerivableFunction::gradient_t grad = c[i]->gradient (x);
          for (unsigned j = 0; j < jac.size2 (); ++j)
            jac (i, j) = grad[j];
        }
    }

  }; // end of namespace detail.
}; // end of namespace optimization.

#endif //! OPTIMIZATION_UTIL_HXX
