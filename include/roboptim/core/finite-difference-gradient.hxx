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

#ifndef ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX
# define ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX
# include "boost/type_traits/is_same.hpp"
# include <boost/mpl/same_as.hpp>

namespace roboptim
{
  template <>
  inline BadGradient<EigenMatrixSparse>::BadGradient
  (const vector_t& x,
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
    assert (analyticalGradient.size () ==
	    finiteDifferenceGradient.size ());

    maxDelta_ = -std::numeric_limits<Function::value_type>::infinity ();
    for (size_type i = 0; i < analyticalGradient.size (); ++i)
      {
	value_type delta =
	  std::fabs (analyticalGradient.coeff (i)
		     - finiteDifferenceGradient.coeff (i));

	if (delta > maxDelta_)
	  {
	    maxDelta_ = delta;
	    maxDeltaComponent_ = i;
	  }
      }
  }

  template <typename T>
  BadGradient<T>::BadGradient (const vector_t& x,
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
    assert (analyticalGradient.size () ==
	    finiteDifferenceGradient.size ());

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

  template <typename T>
  BadGradient<T>::~BadGradient () throw ()
  {}

  template <typename T>
  std::ostream&
  BadGradient<T>::print (std::ostream& o) const throw ()
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

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const BadGradient<T>& bg)
  {
    return bg.print (o);
  }

  template <typename T, typename FdgPolicy>
  GenericFiniteDifferenceGradient<T, FdgPolicy>::GenericFiniteDifferenceGradient
  (const GenericFunction<T>& adaptee,
   typename GenericDifferentiableFunction<T>::value_type epsilon)
    throw ()
    : GenericDifferentiableFunction<T>
      (adaptee.inputSize (), adaptee.outputSize ()),
      FdgPolicy (),
      adaptee_ (adaptee),
      epsilon_ (epsilon),
      xEps_ (adaptee.inputSize ())
  {
    // Avoid meaningless values for epsilon such as 0 or NaN.
    assert (epsilon != 0. && epsilon == epsilon);
  }

  template <typename T, typename FdgPolicy>
  GenericFiniteDifferenceGradient<
    T, FdgPolicy>::~GenericFiniteDifferenceGradient () throw ()
  {
  }

  template <typename T, typename FdgPolicy>
  void
  GenericFiniteDifferenceGradient<T, FdgPolicy>::impl_compute
  (result_t& result, const argument_t& argument) const throw ()
  {
    adaptee_ (result, argument);
  }

  template <typename T, typename FdgPolicy>
  void
  GenericFiniteDifferenceGradient<T, FdgPolicy>::impl_gradient
  (gradient_t& gradient,
   const argument_t& argument,
   size_type idFunction) const throw ()
  {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    Eigen::internal::set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    this->computeGradient (adaptee_, epsilon_, gradient,
			   argument, idFunction, xEps_);
  }

  template <typename T>
  bool
  checkGradient
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::size_type functionId,
   const typename GenericDifferentiableFunction<T>::vector_t& x,
   typename GenericDifferentiableFunction<T>::value_type threshold)
    throw ()
  {
    GenericFiniteDifferenceGradient<T> fdfunction (function);
    typename GenericDifferentiableFunction<T>::gradient_t grad =
      function.gradient (x, functionId);
    typename GenericDifferentiableFunction<T>::gradient_t fdgrad =
      fdfunction.gradient (x, functionId);

    return grad.isApprox (fdgrad, threshold);
    return true;
  }

  template <typename T>
  void
  checkGradientAndThrow
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::size_type functionId,
   const typename GenericDifferentiableFunction<T>::vector_t& x,
   typename GenericDifferentiableFunction<T>::value_type threshold)
    throw (BadGradient<T>)
  {
    GenericFiniteDifferenceGradient<T> fdfunction (function);
    typename GenericFiniteDifferenceGradient<T>::gradient_t grad =
      function.gradient (x, functionId);
    typename GenericFiniteDifferenceGradient<T>::gradient_t fdgrad =
      fdfunction.gradient (x, functionId);

    if (!checkGradient (function, functionId, x, threshold))
      throw BadGradient<T> (x, grad, fdgrad, threshold);
  }

  namespace detail
  {
    template <typename T>
    void
    compute_deriv (const GenericFunction<T>& adaptee,
		   typename GenericFunction<T>::size_type j,
		   double h,
		   double& result,
		   double& round,
		   double& trunc,
		   const typename GenericFunction<T>::argument_t& argument,
		   typename GenericFunction<T>::size_type idFunction,
		   typename GenericFunction<T>::argument_t& xEps);

    /// Algorithm from the Gnu Scientific Library.
    template <typename T>
    void
    compute_deriv (const GenericFunction<T>& adaptee,
		   typename GenericFunction<T>::size_type j,
		   double h,
		   double& result,
		   double& round,
		   double& trunc,
		   const typename GenericFunction<T>::argument_t& argument,
		   typename GenericFunction<T>::size_type idFunction,
		   typename GenericFunction<T>::argument_t& xEps)
    {
      /* Compute the derivative using the 5-point rule (x-h, x-h/2, x,
	 x+h/2, x+h). Note that the central point is not used.

	 Compute the error using the difference between the 5-point and
	 the 3-point rule (x-h,x,x+h). Again the central point is not
	 used. */

      xEps = argument;

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

  } // end of namespace detail.

  namespace finiteDifferenceGradientPolicies
  {
    template <>
    inline void
    Simple<EigenMatrixSparse>::computeGradient
    (const GenericFunction<EigenMatrixSparse>& adaptee,
     value_type epsilon,
     gradient_t& gradient,
     const argument_t& argument,
     size_type idFunction,
     argument_t& xEps) const throw ()
    {
      assert (adaptee.outputSize () - idFunction > 0);
      result_t res = adaptee (argument);
      for (size_type j = 0; j < adaptee.inputSize (); ++j)
	{
	  xEps = argument;
	  xEps[j] += epsilon;
	  result_t resEps = adaptee (xEps);
	  gradient.insert (j) =
	    (resEps[idFunction] - res[idFunction]) / epsilon;
	}
    }

    template <typename T>
    void
    Simple<T>::computeGradient
    (const GenericFunction<T>& adaptee,
     value_type epsilon,
     gradient_t& gradient,
     const argument_t& argument,
     size_type idFunction,
     argument_t& xEps) const throw ()
    {
      assert (adaptee.outputSize () - idFunction > 0);

      result_t res = adaptee (argument);
      for (size_type j = 0; j < adaptee.inputSize (); ++j)
	{
	  xEps = argument;
	  xEps[j] += epsilon;
	  typename GenericFunction<T>::result_t resEps = adaptee (xEps);
	  gradient (j) = (resEps[idFunction] - res[idFunction]) / epsilon;
	}
    }

    template <>
    inline void
    FivePointsRule<EigenMatrixSparse>::computeGradient
    (const GenericFunction<EigenMatrixSparse>& adaptee,
     value_type epsilon,
     gradient_t& gradient,
     const argument_t& argument,
     size_type idFunction,
     argument_t& xEps) const throw ()
    {
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
				 argument, idFunction, xEps);
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
				     argument, idFunction,
				     xEps);
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

	  gradient.insert (j) = r_0;
	}
    }

    template <typename T>
    void
    FivePointsRule<T>::computeGradient
    (const GenericFunction<T>& adaptee,
     value_type epsilon,
     gradient_t& gradient,
     const argument_t& argument,
     size_type idFunction,
     argument_t& xEps) const throw ()
    {
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
				 argument, idFunction, xEps);
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
				     argument, idFunction,
				     xEps);
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

	  gradient[j] = r_0;
	}
    }
  } // end of namespace finiteDifferenceGradientPolicies.

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HXX
