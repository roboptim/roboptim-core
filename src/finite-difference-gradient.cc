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

#include <boost/numeric/ublas/io.hpp>

#include <roboptim/core/indent.hh>
#include <roboptim/core/finite-difference-gradient.hh>

namespace roboptim
{
  BadGradient::BadGradient (const vector_t& x,
			    const gradient_t& analyticalGradient,
			    const gradient_t& finiteDifferenceGradient,
			    const value_type& threshold)
    : std::runtime_error ("bad gradient"),
      x_ (x),
      analyticalGradient_ (analyticalGradient),
      finiteDifferenceGradient_ (finiteDifferenceGradient),
      maxDelta_ (),
      threshold_ (threshold)
  {
    gradient_t delta = analyticalGradient - finiteDifferenceGradient;
    for (unsigned i = 0; i < delta.size (); ++i)
      {
	delta[i] = fabs (delta[i]);
	if (maxDelta_ < delta[i])
	  maxDelta_ = delta[i];
      }
  }

  BadGradient::~BadGradient () throw ()
  {}

  std::ostream&
  BadGradient::print (std::ostream& o) const throw ()
  {
    o << this->what () << incindent << iendl
      << "X: " << x_ << iendl
      << "Analytical gradient: " << analyticalGradient_ << iendl
      << "Finite difference gradient: " << finiteDifferenceGradient_
      << iendl
      << "Max. delta: " << maxDelta_ << iendl
      << "Max. allowed delta): " << threshold_ << decindent;
    return o;
  }

  std::ostream&
  operator<< (std::ostream& o, const BadGradient& bg)
  {
    return bg.print (o);
  }



  FiniteDifferenceGradient::FiniteDifferenceGradient (const Function& adaptee,
						      value_type epsilon)
    throw ()
    : DerivableFunction (adaptee.inputSize (), adaptee.outputSize ()),
      adaptee_ (adaptee),
      epsilon_ (epsilon)
  {
    // Avoid meaningless values for epsilon such as 0 or NaN.
    assert (epsilon != 0. && epsilon == epsilon);
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


  bool
  checkGradient (const DerivableFunction& function,
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

  void
  checkGradientAndThrow (const DerivableFunction& function,
			 int i,
			 const Function::vector_t& x,
			 Function::value_type threshold)
    throw (BadGradient)
  {
    FiniteDifferenceGradient fdfunction (function);
    DerivableFunction::gradient_t grad = function.gradient (x, i);
    DerivableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

    if (!checkGradient (function, i, x, threshold))
      throw BadGradient (x, grad, fdgrad, threshold);
  }

} // end of namespace roboptim
