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

/**
 * \brief Class NTimesDerivable implementation.
 */

#ifndef ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX
# define ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX

namespace roboptim
{
  template <unsigned dorder>
  NTimesDerivableFunction<dorder>::NTimesDerivableFunction (size_type m)
    throw ()
    : TwiceDerivableFunction (1, m)
  {
  }

  template <unsigned dorder>
  NTimesDerivableFunction<dorder>::~NTimesDerivableFunction () throw ()
  {
  }

  template <unsigned dorder>
  typename NTimesDerivableFunction<dorder>::vector_t
  NTimesDerivableFunction<dorder>::operator () (const vector_t& x) const
    throw ()
  {
    assert (x.size () == 1);
    return operator () (x[0]);
  }

  template <unsigned dorder>
  typename NTimesDerivableFunction<dorder>::gradient_t
  NTimesDerivableFunction<dorder>::gradient (const vector_t& x, int i) const
    throw ()
  {
    assert (x.size () == 1);
    vector_t d = derivative (x[0], 1);

    vector_t res (1);
    res[0] = d[i];
    return res;
  }

  template <unsigned dorder>
  typename NTimesDerivableFunction<dorder>::hessian_t
  NTimesDerivableFunction<dorder>::hessian (const vector_t& x, int i)
    const throw ()
  {
    hessian_t hessian (1, 1);

    vector_t dd = derivative (x[0], 2);
    hessian (0, 0) = dd[i];

    return hessian;
  }

  template <unsigned dorder>
  std::ostream&
  NTimesDerivableFunction<dorder>::print (std::ostream& o) const
    throw ()
  {
    o << "Function derivable " << derivabilityOrder << " times.";
    return o;
  }
}

#endif //! ROBOPTIM_CORE_N_TIMES_DERIVABLE_HXX
