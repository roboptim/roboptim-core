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
 * \brief Class NTimesDerivable declaration.
 */

#ifndef ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# define ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# include <boost/static_assert.hpp>

# include <roboptim/core/twice-derivable-function.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_function
     @{
  */

  template <unsigned dorder>
  class NTimesDerivableFunction;

  template <>
  class NTimesDerivableFunction<2> : public TwiceDerivableFunction
  {
  public:
    static const size_type derivabilityOrder = 2;


    virtual ~NTimesDerivableFunction () throw ()
    {}


    /// Return the size of the derivative vector.
    size_type derivativeSize () const throw ()
    {
      return outputSize ();
    }

    /// Check if a derivative is valid (check sizes).
    bool isValidDerivative (const gradient_t& derivative) const throw ()
    {
      return derivative.size () == this->derivativeSize ();
    }


    /// Evaluate the function at a given point.
    result_t operator () (double argument) const
      throw ()
    {
      result_t result (outputSize ());
      (*this) (result, argument);
      return result;
    }


    /// Evaluate the function at a given point.
    void operator () (result_t& result, double argument) const throw ()
    {
      assert (isValidResult (result));
      this->impl_compute (result, argument);
      assert (isValidResult (result));
    }


    /// Compute the derivative of the function at a certain order
    /// and for a given point.
    gradient_t derivative (double argument, size_type order = 1) const
      throw ()
    {
      gradient_t derivative (derivativeSize ());
      this->derivative (derivative, argument, order);
      return derivative;
    }


    /// Compute the derivative of the function at a certain order
    /// and for a given point.
    void derivative (gradient_t derivative,
		     double argument,
		     size_type order = 1) const
      throw ()
    {
      assert (order <= derivabilityOrder
	      && isValidDerivative (derivative));
      this->impl_derivative (derivative, argument, order);
      assert (isValidDerivative (derivative));
    }


    virtual std::ostream& print (std::ostream& o) const throw ()
    {
      o << "Function derivable " << derivabilityOrder << " times.";
      return o;
    }

  protected:
    /// Constructor.
    NTimesDerivableFunction (size_type outputSize = 1) throw ()
      : TwiceDerivableFunction (1, outputSize)
    {}

    /// Implement the generic evaluation method, as required by the Function
    /// class, using the specific operator () method declared in this class.
    void impl_compute (result_t& result, const argument_t& argument)
      const throw ()
    {
      (*this) (result, argument[0]);
    }

    /// Evaluate the function, has to be implemented by the concrete class.
    virtual void impl_compute (result_t&, double) const throw () = 0;

    /// Implement the generic gradient computation method, as required by
    /// the DerivableFunction class, using the derivative method declared
    /// in this class.
    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			int functionId = 0) const throw ()
    {
      assert (functionId == 0);
      this->derivative (gradient, argument[0], 1);
    }


    /// Derivative computation, has to be implemented in concrete class.
    virtual void impl_derivative (gradient_t& derivative,
				  double argument,
				  size_type order = 1) const throw () = 0;

    void impl_hessian (hessian_t& hessian,
		       const argument_t& argument,
		       int functionId = 0) const throw ()
    {
      assert (functionId == 0);

      gradient_t gradient (gradientSize ());
      this->derivative (gradient, argument[0], 2);

      hessian (0, 0) = gradient[functionId];
    }
  };

  /// \brief Define a \f$\mathbb{R} \rightarrow \mathbb{R}^m\f$ function,
  /// derivable n times (n >= 2).
  template <unsigned DerivabilityOrder>
  class NTimesDerivableFunction
    : public NTimesDerivableFunction<DerivabilityOrder - 1>
  {
  public:
    BOOST_STATIC_ASSERT(DerivabilityOrder > 2);

    typedef Function::size_type size_type;
    static const size_type derivabilityOrder = DerivabilityOrder;

    virtual ~NTimesDerivableFunction () throw ();

    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    /// Constructor.
    NTimesDerivableFunction (size_type m = 1) throw ()
      : NTimesDerivableFunction<DerivabilityOrder - 1> (m)
    {}
  };

  /**
     @}
  */
}
# include <roboptim/core/n-times-derivable-function.hxx>
#endif //! ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
