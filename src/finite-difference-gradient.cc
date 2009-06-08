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

#include <roboptim/core/finite-difference-gradient.hh>

namespace roboptim
{
  FiniteDifferenceGradient::FiniteDifferenceGradient (const Function& adaptee,
						      value_type epsilon)
    throw ()
    : DerivableFunction (adaptee.inputSize (), adaptee.outputSize ()),
      adaptee_ (adaptee),
      epsilon_ (epsilon)
  {
    assert (epsilon != 0.);
  }

  FiniteDifferenceGradient::~FiniteDifferenceGradient () throw ()
  {
  }

  void
  FiniteDifferenceGradient::impl_compute (result_t& result,
					  const argument_t& argument)
    const throw ()
  {
    adaptee_ (result, argument);
  }

  void
  FiniteDifferenceGradient::impl_gradient (gradient_t& gradient,
					   const argument_t& argument,
					   size_type idFunction) const throw ()
  {
    assert (outputSize () - idFunction > 0);

    result_t res = adaptee_ (argument);

    for (unsigned j = 0; j < inputSize (); ++j)
      {
	argument_t xEps = argument;
	xEps[j] += epsilon_;
	result_t resEps = adaptee_ (xEps);

	gradient (j) = (resEps[idFunction] - res[idFunction]) / epsilon_;
      }
  }


  bool checkGradient (DerivableFunction& function,
		      int i,
		      const Function::vector_t& x,
		      Function::value_type threshold) throw ()
  {
    FiniteDifferenceGradient fdfunction (function);
    DerivableFunction::gradient_t grad = function.gradient (x, i);
    DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

    for (unsigned col = 0; col < function.inputSize (); ++col)
      if (fabs (grad[col] - fdgrad[col]) >= threshold)
	return false;
    return true;
  }

} // end of namespace roboptim
