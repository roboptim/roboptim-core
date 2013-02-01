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

#include <limits>

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
      maxDeltaComponent_ (),
      threshold_ (threshold)
  {
    assert (analyticalGradient.size () == finiteDifferenceGradient.size ());

    maxDelta_ = -std::numeric_limits<Function::value_type>::infinity ();
    for (size_type i = 0; i < analyticalGradient.size (); ++i)
      {
	value_type delta =
	  std::fabs (analyticalGradient[i] - finiteDifferenceGradient[i]);

	if (delta > maxDelta_)
	  {
	    maxDelta_ = delta;
	    maxDeltaComponent_ = i;
	  }
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
      << "Max. delta in component: " << maxDeltaComponent_ << iendl
      << "Max. allowed delta: " << threshold_ << decindent;
    return o;
  }

  std::ostream&
  operator<< (std::ostream& o, const BadGradient& bg)
  {
    return bg.print (o);
  }


  namespace detail
  {
    ROBOPTIM_DLLLOCAL void
    compute_deriv (const Function& adaptee,
		   Function::size_type j,
		   double h,
		   double& result,
		   double& round,
		   double& trunc,
		   const Function::argument_t& argument,
		   Function::size_type idFunction);

    /// Algorithm from the Gnu Scientific Library.
    ROBOPTIM_DLLLOCAL void
    compute_deriv (const Function& adaptee,
		   Function::size_type j,
		   double h,
		   double& result,
		   double& round,
		   double& trunc,
		   const Function::argument_t& argument,
		   Function::size_type idFunction)
    {
      /* Compute the derivative using the 5-point rule (x-h, x-h/2, x,
	 x+h/2, x+h). Note that the central point is not used.

	 Compute the error using the difference between the 5-point and
	 the 3-point rule (x-h,x,x+h). Again the central point is not
	 used. */

      Function::argument_t xEps = argument;

      xEps[j] = argument[j] - h;
      double fm1 = adaptee (xEps)[idFunction];
      xEps[j] = argument[j] + h;
      double fp1 = adaptee (xEps)[idFunction];
      xEps[j] = argument[j] - (h / 2.);
      double fmh = adaptee (xEps)[idFunction];
      xEps[j] = argument[j] + (h / 2.);
      double fph = adaptee (xEps)[idFunction];

      double r3 = .5 * (fp1 - fm1);
      double r5 = (4. / 3.) * (fph - fmh) - (1. / 3.) * r3;

      double e3 = (std::fabs (fp1) + std::fabs (fm1))
	* std::numeric_limits<double>::epsilon ();
      double e5 = 2. * (std::fabs (fph) + std::fabs (fmh))
	* std::numeric_limits<double>::epsilon () + e3;

      /* The next term is due to finite precision in x+h = O (eps * x) */

      double dy =
	std::max (std::fabs (r3 / h), std::fabs (r5 / h))
	* (std::fabs (argument[j]) / h)
	* std::numeric_limits<double>::epsilon ();

      /* The truncation error in the r5 approximation itself is O(h^4).
	 However, for safety, we estimate the error from r5-r3, which is
	 O(h^2).  By scaling h we will minimise this estimated error, not
	 the actual truncation error in r5. */

      result = r5 / h;
      trunc = std::fabs ((r5 - r3) / h); // Estimated truncation error O(h^2)
      round = std::fabs (e5 / h) + dy; // Rounding error (cancellations)
    }
  }

  namespace finiteDifferenceGradientPolicies
  {
    void
    Simple::computeGradient
    (const Function& adaptee,
     Function::value_type epsilon,
     Function::result_t& gradient,
     const Function::argument_t& argument,
     Function::size_type idFunction) const throw ()
    {
      typedef Function::value_type value_type;
      assert (adaptee.outputSize () - idFunction > 0);

      Function::result_t res = adaptee (argument);
      for (size_type j = 0; j < adaptee.inputSize (); ++j)
	{
	  Function::argument_t xEps = argument;
	  xEps[j] += epsilon;
	  Function::result_t resEps = adaptee (xEps);
	  gradient (j) = (resEps[idFunction] - res[idFunction]) / epsilon;
	}
    }

    void
    FivePointsRule::computeGradient
    (const Function& adaptee,
     Function::value_type epsilon,
     Function::result_t& gradient,
     const Function::argument_t& argument,
     Function::size_type idFunction) const throw ()
    {
      typedef Function::value_type value_type;

      assert (adaptee.outputSize () - idFunction > 0);

      value_type h = epsilon / 2.;
      value_type r_0 = 0.;
      value_type round = 0.;
      value_type trunc = 0.;
      value_type error = 0.;

      for (size_type j = 0; j < argument.size (); ++j)
	{
	  detail::compute_deriv (adaptee, j, h,
				 r_0, round, trunc,
				 argument, idFunction);
	  error = round + trunc;

	  if (round < trunc && (round > 0 && trunc > 0))
	    {
	      value_type r_opt = 0., round_opt = 0., trunc_opt = 0.,
		error_opt = 0.;

	      /* Compute an optimised stepsize to minimize the total error,
		 using the scaling of the truncation error (O(h^2)) and
		 rounding error (O(1/h)). */

	      value_type h_opt =
		h * std::pow (round / (2. * trunc), 1. / 3.);

	      detail::compute_deriv (adaptee, j, h_opt,
				     r_opt, round_opt, trunc_opt,
				     argument, idFunction);
	      error_opt = round_opt + trunc_opt;

	      /* Check that the new error is smaller, and that the new
		 derivative is consistent with the error bounds of the
		 original estimate. */

	      if (error_opt < error && std::fabs (r_opt - r_0) < 4. * error)
		{
		  r_0 = r_opt;
		  error = error_opt;
		}
	    }

	  gradient (j) = r_0;
	}
    }
  }


  bool
  checkGradient (const DifferentiableFunction& function,
		 Function::size_type i,
		 const Function::vector_t& x,
		 Function::value_type threshold) throw ()
  {
    FiniteDifferenceGradient<> fdfunction (function);
    DifferentiableFunction::gradient_t grad = function.gradient (x, i);
    DifferentiableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

    for (Function::size_type col = 0; col < function.inputSize (); ++col)
      if (fabs (grad[col] - fdgrad[col]) >= threshold)
	return false;
    return true;
  }

  void
  checkGradientAndThrow (const DifferentiableFunction& function,
			 Function::size_type i,
			 const Function::vector_t& x,
			 Function::value_type threshold)
    throw (BadGradient)
  {
    FiniteDifferenceGradient<> fdfunction (function);
    DifferentiableFunction::gradient_t grad = function.gradient (x, i);
    DifferentiableFunction::gradient_t fdgrad = fdfunction.gradient (x, i);

    if (!checkGradient (function, i, x, threshold))
      throw BadGradient (x, grad, fdgrad, threshold);
  }

} // end of namespace roboptim
