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
 * \brief Implementation of the FiniteDifferenceGradient class.
 */

#include <roboptim/core/finite-difference-gradient.hh>

namespace roboptim
{
  FiniteDifferenceGradient::FiniteDifferenceGradient (const Function& adaptee,
						      value_type epsilon)
    throw ()
    : DerivableFunction (adaptee.n, adaptee.m),
      adaptee_ (adaptee),
      epsilon_ (epsilon)
  {
    assert (epsilon != 0.);
  }

  FiniteDifferenceGradient::~FiniteDifferenceGradient () throw ()
  {
  }

  FiniteDifferenceGradient::vector_t
  FiniteDifferenceGradient:: operator () (const vector_t& x) const throw ()
  {
    return adaptee_ (x);
  }

  FiniteDifferenceGradient::gradient_t
  FiniteDifferenceGradient::gradient (const vector_t& x, int i) const throw ()
  {
    assert (m - i > 0);
    gradient_t gradient (n);

    vector_t res = adaptee_ (x);

    for (unsigned j = 0; j < n; ++j)
      {
	vector_t xEps = x;
	xEps[j] += epsilon_;
	vector_t resEps = adaptee_ (xEps);

	gradient (j) = (resEps[i] - res[i]) / epsilon_;
      }
    return gradient;
  }


  bool checkGradient (DerivableFunction& function,
		      int i,
		      const Function::vector_t& x,
		      Function::value_type threshold) throw ()
  {
    FiniteDifferenceGradient fdfunction (function);
    DerivableFunction::gradient_t grad = function.gradient (x, i);
    DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

    for (unsigned i = 0; i < function.n; ++i)
      if (fabs (grad[i] - fdgrad[i]) >= threshold)
	return false;
    return true;
  }

} // end of namespace roboptim
