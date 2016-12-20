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

#ifndef ROBOPTIM_CORE_DECORATOR_FINITE_DIFFERENCE_GRADIENT_HXX
# define ROBOPTIM_CORE_DECORATOR_FINITE_DIFFERENCE_GRADIENT_HXX

# include <stdexcept>

# include <boost/type_traits/is_same.hpp>
# include <boost/mpl/same_as.hpp>
# include <boost/format.hpp>

# include <roboptim/core/util.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  template <>
  inline BadGradient<EigenMatrixSparse>::BadGradient
  (const_argument_ref x,
   const_gradient_ref analyticalGradient,
   const_gradient_ref finiteDifferenceGradient,
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
  BadGradient<T>::BadGradient (const_argument_ref x,
			       const_gradient_ref analyticalGradient,
			       const_gradient_ref finiteDifferenceGradient,
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
  BadGradient<T>::print (std::ostream& o) const
  {
    o << this->what () << incindent << iendl
      << "X: " << x_ << iendl
      << "Analytical gradient: " << toDense (analyticalGradient_) << iendl
      << "Finite difference gradient: " << toDense (finiteDifferenceGradient_)
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
  (const boost::shared_ptr<const GenericFunction<T> >& adaptee,
   typename GenericDifferentiableFunction<T>::value_type epsilon)
    : GenericDifferentiableFunction<T>
      (adaptee->inputSize (), adaptee->outputSize (), generateName (*adaptee)),
    FdgPolicy (*adaptee),
    adaptee_ (adaptee),
    epsilon_ (epsilon),
    xEps_ (adaptee->inputSize ())
  {
    // Avoid meaningless values for epsilon such as 0 or NaN.
    assert (epsilon != 0. && epsilon == epsilon);
  }

  template <typename T, typename FdgPolicy>
  GenericFiniteDifferenceGradient<T, FdgPolicy>::GenericFiniteDifferenceGradient
  (const GenericFunction<T>& adaptee,
   typename GenericDifferentiableFunction<T>::value_type epsilon)
    : GenericDifferentiableFunction<T>
      (adaptee.inputSize (), adaptee.outputSize (), generateName (adaptee)),
    FdgPolicy (adaptee),
    adaptee_ (&adaptee, detail::NoopDeleter<GenericFunction<T> > ()),
    epsilon_ (epsilon),
    xEps_ (adaptee.inputSize ())
  {
    // Avoid meaningless values for epsilon such as 0 or NaN.
    assert (epsilon != 0. && epsilon == epsilon);
  }

  template <>
  inline
  BadJacobian<EigenMatrixSparse>::BadJacobian
  (const_argument_ref x,
   const_jacobian_ref analyticalJacobian,
   const_jacobian_ref finiteDifferenceJacobian,
   const value_type& threshold)
    : std::runtime_error ("bad jacobian"),
      x_ (x),
      analyticalJacobian_ (analyticalJacobian),
      finiteDifferenceJacobian_ (finiteDifferenceJacobian),
      maxDelta_ (),
      maxDeltaRow_ (),
      maxDeltaCol_ (),
      threshold_ (threshold)
  {
    assert (analyticalJacobian.rows () == finiteDifferenceJacobian.rows ());
    assert (analyticalJacobian.cols () == finiteDifferenceJacobian.cols ());

    maxDelta_ = -std::numeric_limits<Function::value_type>::infinity ();
    for (size_type i = 0; i < analyticalJacobian.rows (); ++i)
      for (size_type j = 0; j < analyticalJacobian.cols (); ++j)
	{
          value_type delta =
	    std::fabs (analyticalJacobian.coeff (i,j)
		       - finiteDifferenceJacobian.coeff (i,j));

          if (delta > maxDelta_)
	    {
              maxDelta_ = delta;
              maxDeltaRow_ = i;
              maxDeltaCol_ = j;
	    }
	}
  }


  template <typename T>
  BadJacobian<T>::BadJacobian (const_argument_ref x,
                               const_jacobian_ref analyticalJacobian,
                               const_jacobian_ref finiteDifferenceJacobian,
                               const value_type& threshold)
    : std::runtime_error ("bad jacobian"),
      x_ (x),
      analyticalJacobian_ (analyticalJacobian),
      finiteDifferenceJacobian_ (finiteDifferenceJacobian),
      maxDelta_ (),
      maxDeltaRow_ (),
      maxDeltaCol_ (),
      threshold_ (threshold)
  {
    assert (analyticalJacobian.rows () == finiteDifferenceJacobian.rows ());
    assert (analyticalJacobian.cols () == finiteDifferenceJacobian.cols ());

    maxDelta_ = -std::numeric_limits<Function::value_type>::infinity ();
    for (size_type i = 0; i < analyticalJacobian.rows (); ++i)
      for (size_type j = 0; j < analyticalJacobian.cols (); ++j)
	{
          value_type delta =
	    std::fabs (analyticalJacobian (i,j)
		       - finiteDifferenceJacobian (i,j));

          if (delta > maxDelta_)
	    {
              maxDelta_ = delta;
              maxDeltaRow_ = i;
              maxDeltaCol_ = j;
	    }
	}
  }

  template <typename T>
  BadJacobian<T>::~BadJacobian () throw ()
  {}

  template <typename T>
  std::ostream&
  BadJacobian<T>::print (std::ostream& o) const
  {
    o << this->what () << incindent << iendl
      << "X: " << x_ << iendl
      << "Analytical Jacobian: " << toDense (analyticalJacobian_) << iendl
      << "Finite difference Jacobian: " << toDense (finiteDifferenceJacobian_)
      << iendl
      << "Max. delta: " << maxDelta_ << iendl
      << "Max. delta in row: " << maxDeltaRow_ << iendl
      << "Max. delta in column: " << maxDeltaCol_ << iendl
      << "Max. allowed delta: " << threshold_ << decindent;
    return o;
  }

  template <typename T>
  std::ostream&
  operator<< (std::ostream& o, const BadJacobian<T>& bj)
  {
    return bj.print (o);
  }

  template <typename T, typename FdgPolicy>
  GenericFiniteDifferenceGradient<T, FdgPolicy>::~GenericFiniteDifferenceGradient ()
  {
  }

  template <typename T, typename FdgPolicy>
  void
  GenericFiniteDifferenceGradient<T, FdgPolicy>::impl_compute
  (result_ref result, const_argument_ref argument) const
  {
    (*adaptee_) (result, argument);
  }

  template <typename T, typename FdgPolicy>
  void
  GenericFiniteDifferenceGradient<T, FdgPolicy>::impl_gradient
  (gradient_ref gradient,
   const_argument_ref argument,
   size_type idFunction) const
  {
    this->computeGradient (epsilon_, gradient,
			   argument, idFunction, xEps_);
  }

  template <typename T, typename FdgPolicy>
  void
  GenericFiniteDifferenceGradient<T, FdgPolicy>::impl_jacobian
  (jacobian_ref jacobian,
   const_argument_ref argument) const
  {
    this->computeJacobian(epsilon_, jacobian, argument, xEps_);
  }

  template <typename T, typename FdgPolicy>
  std::ostream&
  GenericFiniteDifferenceGradient<T, FdgPolicy>::print (std::ostream& o) const
  {
    o << this->getName () << ":" << incindent
      << iendl << "Wrapped function: " << *adaptee_
      << decindent;

    return o;
  }

  template <typename T, typename FdgPolicy>
  std::string
  GenericFiniteDifferenceGradient<T, FdgPolicy>::generateName
  (const GenericFunction<T>& adaptee) const
  {
    if (!adaptee.getName ().empty ())
      return (boost::format ("%s (finite differences)")
              % adaptee.getName ()).str ();
    else return "Finite difference wrapper";
  }

  template <typename T>
  bool
  checkGradient
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::size_type functionId,
   typename GenericDifferentiableFunction<T>::const_argument_ref x,
   typename GenericDifferentiableFunction<T>::value_type threshold,
   typename GenericDifferentiableFunction<T>::value_type fd_eps)
  {
    ROBOPTIM_ALLOW_DEPRECATED_ON;
    GenericFiniteDifferenceGradient<T> fdfunction (function, fd_eps);
    ROBOPTIM_ALLOW_DEPRECATED_OFF;

    typename GenericDifferentiableFunction<T>::gradient_t grad =
      function.gradient (x, functionId);
    typename GenericDifferentiableFunction<T>::gradient_t fdgrad =
      fdfunction.gradient (x, functionId);

    return allclose(grad, fdgrad, threshold, threshold);
  }

  template <typename T>
  void
  checkGradientAndThrow
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::size_type functionId,
   typename GenericDifferentiableFunction<T>::const_argument_ref x,
   typename GenericDifferentiableFunction<T>::value_type threshold,
   typename GenericDifferentiableFunction<T>::value_type fd_eps)
    throw (BadGradient<T>)
  {
    ROBOPTIM_ALLOW_DEPRECATED_ON;
    GenericFiniteDifferenceGradient<T> fdfunction (function, fd_eps);
    ROBOPTIM_ALLOW_DEPRECATED_OFF;

    typename GenericFiniteDifferenceGradient<T>::gradient_t grad =
      function.gradient (x, functionId);
    typename GenericFiniteDifferenceGradient<T>::gradient_t fdgrad =
      fdfunction.gradient (x, functionId);

    if (!allclose(grad, fdgrad, threshold, threshold))
      throw BadGradient<T> (x, grad, fdgrad, threshold);
  }

  template <typename T>
  bool
  checkJacobian
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::const_argument_ref x,
   typename GenericDifferentiableFunction<T>::value_type threshold,
   typename GenericDifferentiableFunction<T>::value_type fd_eps)
  {
    ROBOPTIM_ALLOW_DEPRECATED_ON;
    GenericFiniteDifferenceGradient<T> fdfunction (function, fd_eps);
    ROBOPTIM_ALLOW_DEPRECATED_OFF;

    typename GenericDifferentiableFunction<T>::jacobian_t jac =
      function.jacobian (x);
    typename GenericDifferentiableFunction<T>::jacobian_t fdjac =
      fdfunction.jacobian (x);

    return allclose(jac, fdjac, threshold, threshold);
  }

  template <typename T>
  void
  checkJacobianAndThrow
  (const GenericDifferentiableFunction<T>& function,
   typename GenericDifferentiableFunction<T>::const_argument_ref x,
   typename GenericDifferentiableFunction<T>::value_type threshold,
   typename GenericDifferentiableFunction<T>::value_type fd_eps)
    throw (BadJacobian<T>)
  {
    ROBOPTIM_ALLOW_DEPRECATED_ON;
    GenericFiniteDifferenceGradient<T> fdfunction (function, fd_eps);
    ROBOPTIM_ALLOW_DEPRECATED_OFF;

    typename GenericDifferentiableFunction<T>::jacobian_t jac =
      function.jacobian (x);
    typename GenericDifferentiableFunction<T>::jacobian_t fdjac =
      fdfunction.jacobian (x);

    if (!allclose(jac, fdjac, threshold, threshold))
      throw BadJacobian<T> (x, jac, fdjac, threshold);
  }

  namespace finiteDifferenceGradientPolicies
  {
    /// Algorithm from the Gnu Scientific Library.
    template <typename T>
    void
    FivePointsRule<T>::compute_deriv
    (typename GenericFunction<T>::size_type j,
     double h,
     double& result,
     double& round,
     double& trunc,
     typename GenericFunction<T>::const_argument_ref argument,
     typename GenericFunction<T>::size_type idFunction,
     typename GenericFunction<T>::argument_ref xEps) const
    {
      /* Compute the derivative using the 5-point rule (x-h, x-h/2, x,
	 x+h/2, x+h). Note that the central point is not used.

	 Compute the error using the difference between the 5-point and
	 the 3-point rule (x-h,x,x+h). Again the central point is not
	 used. */

      xEps = argument;

      xEps[j] = argument[j] - h;
      this->adaptee_ (tmpResult_, xEps);
      double fm1 = tmpResult_[idFunction];

      xEps[j] = argument[j] + h;
      this->adaptee_ (tmpResult_, xEps);
      double fp1 = tmpResult_[idFunction];

      xEps[j] = argument[j] - (h / 2.);
      this->adaptee_ (tmpResult_, xEps);
      double fmh = tmpResult_[idFunction];

      xEps[j] = argument[j] + (h / 2.);
      this->adaptee_ (tmpResult_, xEps);
      double fph = tmpResult_[idFunction];

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


    template <>
    inline void
    Policy<EigenMatrixSparse>::computeJacobian
    (value_type epsilon,
     jacobian_ref jacobian,
     const_argument_ref argument,
     argument_ref xEps) const
    {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      bool cur_malloc_allowed = is_malloc_allowed ();
      set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      typedef Eigen::Triplet<double> triplet_t;

      std::vector<triplet_t> coefficients;

      // For each column
#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
      for (jacobian_t::StorageIndex j = 0; j < this->adaptee_.inputSize (); ++j)
#else
      for (jacobian_t::Index j = 0; j < this->adaptee_.inputSize (); ++j)
#endif
        {
          gradient_t col (this->adaptee_.outputSize ());

          computeColumn (epsilon, col, argument, j, xEps);

#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
          const matrix_t::StorageIndex j_ = j;
#else
          const matrix_t::Index j_ = static_cast<const matrix_t::Index> (j);
#endif
          for (gradient_t::InnerIterator it (col); it; ++it)
            {
#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
              const matrix_t::StorageIndex idx = it.index ();
#else
              const matrix_t::Index idx =
                static_cast<const matrix_t::Index> (it.index ());
#endif

#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
              assert (idx < this->adaptee_.outputSize ());
#else
              assert (idx < static_cast<const matrix_t::Index>
                      (this->adaptee_.outputSize ()));
#endif

              coefficients.push_back
		(triplet_t (idx, j_, it.value ()));
	    }
	}
      jacobian.setFromTriplets (coefficients.begin (), coefficients.end ());

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    }

    template <typename T>
    void
    Policy<T>::computeJacobian
    (value_type epsilon,
     jacobian_ref jacobian,
     const_argument_ref argument,
     argument_ref xEps) const
    {
      // For each Jacobian column
      for (typename jacobian_t::Index j = 0;
	   j < this->adaptee_.inputSize(); ++j)
	{
          column_.setZero();
          computeColumn (epsilon, column_, argument, j, xEps);
          jacobian.col (j) = column_;
	}
    }

    template <>
    inline void
    Simple<EigenMatrixSparse>::computeGradient
    (value_type epsilon,
     gradient_ref gradient,
     const_argument_ref argument,
     size_type idFunction,
     argument_ref xEps) const
    {
      assert (adaptee_.outputSize () - idFunction > 0);
      adaptee_ (result_, argument);
      for (size_type j = 0; j < adaptee_.inputSize (); ++j)
	{
	  xEps = argument;
	  xEps[j] += epsilon;
	  adaptee_ (resultEps_, xEps);
	  gradient.insert (j) =
	    (resultEps_[idFunction] - result_[idFunction]) / epsilon;
	}
    }

    template <typename T>
    void
    Simple<T>::computeGradient
    (value_type epsilon,
     gradient_ref gradient,
     const_argument_ref argument,
     size_type idFunction,
     argument_ref xEps) const
    {
      assert (this->adaptee_.outputSize () - idFunction > 0);

      this->adaptee_ (result_, argument);
      for (size_type j = 0; j < this->adaptee_.inputSize (); ++j)
	{
	  xEps = argument;
	  xEps[j] += epsilon;
	  this->adaptee_ (resultEps_, xEps);
	  gradient (j) =
	    (resultEps_[idFunction] - result_[idFunction]) / epsilon;
	}
    }


    template <>
    inline void
    Simple<EigenMatrixSparse>::computeColumn
    (value_type epsilon,
     gradient_ref column,
     const_argument_ref argument,
     size_type colIdx,
     argument_ref xEps) const
    {
      assert (adaptee_.inputSize () - colIdx > 0);
      assert (resultEps_.size () == adaptee_.outputSize ());
      assert (result_.size () == adaptee_.outputSize ());

      // Note: result_ = f(x) should have been called already
      xEps = argument;
      xEps[colIdx] += epsilon;
      adaptee_ (resultEps_, xEps);
      // Note: actual zeros may also be added to the sparse matrix to keep the
      // sparse pattern constant.
      column = ((resultEps_ - result_) / epsilon).sparseView (-1., this->sparseEps_);
    }

    template <typename T>
    void
    Simple<T>::computeColumn
    (value_type epsilon,
     gradient_ref column,
     const_argument_ref argument,
     size_type colIdx,
     argument_ref xEps) const
    {
      assert (this->adaptee_.inputSize () - colIdx > 0);
      assert (this->resultEps_.size () == this->adaptee_.outputSize ());
      assert (this->result_.size () == this->adaptee_.outputSize ());

      // Note: result_ = f(x) should have been called already
      xEps = argument;
      xEps[colIdx] += epsilon;
      this->adaptee_ (resultEps_, xEps);
      column = (resultEps_ - result_) / epsilon;
    }

    template <typename T>
    void
    Simple<T>::computeJacobian
    (value_type epsilon,
     jacobian_ref jacobian,
     const_argument_ref argument,
     argument_ref xEps) const
    {
      // Data used by each computeColumn
      this->adaptee_ (result_, argument);

      // Call parent Jacobian
      policy_t::computeJacobian (epsilon, jacobian, argument, xEps);
    }

    template <>
    inline void
    FivePointsRule<EigenMatrixSparse>::computeGradient
    (value_type epsilon,
     gradient_ref gradient,
     const_argument_ref argument,
     size_type idFunction,
     argument_ref xEps) const
    {
      assert (this->adaptee_.outputSize () - idFunction > 0);

      value_type h = epsilon / 2.;
      value_type r_0 = 0.;
      value_type round = 0.;
      value_type trunc = 0.;
      value_type error = 0.;

      for (size_type j = 0; j < argument.size (); ++j)
	{
	  this->compute_deriv (j, h,
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

	      this->compute_deriv (j, h_opt,
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
		}
	    }

	  gradient.insert (j) = r_0;
	}
    }

    template <typename T>
    void
    FivePointsRule<T>::computeGradient
    (value_type epsilon,
     gradient_ref gradient,
     const_argument_ref argument,
     size_type idFunction,
     argument_ref xEps) const
    {
      assert (this->adaptee_.outputSize () - idFunction > 0);

      value_type h = epsilon / 2.;
      value_type r_0 = 0.;
      value_type round = 0.;
      value_type trunc = 0.;
      value_type error = 0.;

      for (size_type j = 0; j < argument.size (); ++j)
	{
	  this->compute_deriv (j, h,
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

	      this->compute_deriv (j, h_opt,
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
		}
	    }

	  gradient[j] = r_0;
	}
    }


    template <>
    inline void FivePointsRule<EigenMatrixSparse>::computeJacobian
    (value_type epsilon,
     jacobian_ref jacobian,
     const_argument_ref argument,
     argument_ref xEps) const
    {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      bool cur_malloc_allowed = is_malloc_allowed ();
      set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION

      typedef Eigen::Triplet<double> triplet_t;

      std::vector<triplet_t> coefficients;
#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
      for (jacobian_t::StorageIndex i = 0; i < this->adaptee_.outputSize (); ++i)
#else
      for (jacobian_t::Index i = 0; i < this->adaptee_.outputSize (); ++i)
#endif
        {
          gradient_t grad (this->adaptee_.inputSize ());

          computeGradient (epsilon, grad, argument, i, xEps);

#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
          const matrix_t::StorageIndex i_ = i;
#else
          const matrix_t::Index i_ = static_cast<const matrix_t::Index> (i);
#endif
          for (gradient_t::InnerIterator it (grad); it; ++it)
            {
#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
              const matrix_t::StorageIndex idx = it.index ();
#else
              const matrix_t::Index idx =
                static_cast<const matrix_t::Index> (it.index ());
#endif

#if EIGEN_VERSION_AT_LEAST(3, 2, 90)
              assert (idx < this->adaptee_.inputSize ());
#else
              assert (idx < static_cast<const matrix_t::Index>
                      (this->adaptee_.inputSize ()));
#endif

              coefficients.push_back
		(triplet_t (i_, idx, it.value ()));
	    }
	}
      jacobian.setFromTriplets (coefficients.begin (), coefficients.end ());

#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      set_is_malloc_allowed (cur_malloc_allowed);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    }

    template <typename T>
    void FivePointsRule<T>::computeJacobian
    (value_type epsilon,
     jacobian_ref jacobian,
     const_argument_ref argument,
     argument_ref xEps) const
    {
      for (typename jacobian_t::Index i = 0;
	   i < this->adaptee_.outputSize(); ++i)
	{
          this->gradient_.setZero();
          computeGradient (epsilon, this->gradient_, argument, i, xEps);
          jacobian.row (i) = this->gradient_;
	}
    }


    template <>
    inline void
    FivePointsRule<EigenMatrixSparse>::computeColumn
    (value_type /*epsilon*/,
     gradient_ref /*column*/,
     const_argument_ref /*argument*/,
     size_type /*colIdx*/,
     argument_ref /*xEps*/) const
    {
      // TODO: implement for the column-wise Jacobian computation
      throw std::runtime_error ("not implemented");
    }


    template <typename T>
    void
    FivePointsRule<T>::computeColumn
    (value_type /*epsilon*/,
     gradient_ref /*column*/,
     const_argument_ref /*argument*/,
     size_type /*colIdx*/,
     argument_ref /*xEps*/) const
    {
      // TODO: implement for the column-wise Jacobian computation
      throw std::runtime_error ("not implemented");
    }
  } // end of namespace finiteDifferenceGradientPolicies.

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_DECORATOR_FINITE_DIFFERENCE_GRADIENT_HXX
