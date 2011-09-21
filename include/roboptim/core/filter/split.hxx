// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_FILTER_SPLIT_HXX
# define ROBOPTIM_CORE_FILTER_SPLIT_HXX
# include <boost/format.hpp>

# include <roboptim/core/derivative-size.hh>

namespace roboptim
{
  namespace
  {
    template <typename T>
    std::string splitName (const T& fct);

    template <typename T>
    std::string splitName (const T& fct, unsigned functionId)
    {
      boost::format fmt ("%1% (split, function Id = %2%)");
      fmt % fct.getName () % functionId;
      return fmt.str ();
    }
  } // end of anonymous namespace.

  template <typename T>
  Split<T>::Split (boost::shared_ptr<const T> fct, unsigned functionId) throw ()
    : T (fct->inputSize (), 1, splitName (*fct, functionId)),
      function_ (fct),
      functionId_ (functionId)
  {
    assert (functionId < fct->outputSize ());
  }

  template <typename T>
  Split<T>::~Split () throw ()
  {
  }

  template <typename T>
  void
  Split<T>::impl_compute (result_t& result,
			  const argument_t& argument)
    const throw ()
  {
    result_t res (function_->outputSize ());
    (*function_) (res, argument);
    result[0] = res[functionId_];
  }


  template <>
  void
  Split<Function>::impl_gradient (gradient_t&, const argument_t&, size_type)
    const throw ()
  {
    assert (0);
  }

  template <typename T>
  void
  Split<T>::impl_gradient (gradient_t& gradient,
			   const argument_t& argument,
			   size_type functionId)
    const throw ()
  {
    assert (functionId == 0);
    function_->gradient (gradient, argument, functionId_);
  }




  template <>
  void
  Split<Function>::impl_hessian
  (hessian_t&, const argument_t&, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  Split<DerivableFunction>::impl_hessian
  (hessian_t&, const argument_t&, size_type) const throw ()
  {
    assert (0);
  }



  template <typename T>
  void
  Split<T>::impl_hessian (hessian_t& hessian,
			  const argument_t& argument,
			  size_type functionId)
    const throw ()
  {
    assert (functionId == 0);
    function_->hessian (hessian, argument, functionId_);
  }


  template <>
  void
  Split<Function>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  Split<DerivableFunction>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <>
  void
  Split<TwiceDerivableFunction>::impl_derivative
  (gradient_t&, double, size_type) const throw ()
  {
    assert (0);
  }

  template <typename T>
  void
  Split<T>::impl_derivative (gradient_t& derivative,
			     double argument,
			     size_type order)
    const throw ()
  {
    function_->derivative (derivative, argument, order);
  }

  template <typename P, typename C>
  void addNonScalarConstraint
  (P& problem,
   boost::shared_ptr<C> constraint,
   std::vector<Function::interval_t> interval,
   std::vector<Function::value_type> scale)
    throw (std::runtime_error)
  {
    assert (constraint);
    assert (interval.size () == constraint->outputSize ());
    assert (scale.empty () || scale.size () == constraint->outputSize ());

    if (constraint->outputSize () == 1)
    {
      if (scale.empty ())
	problem.addConstraint (constraint, interval[0]);
      else
	problem.addConstraint (constraint, interval[0], scale[0]);
      return;
    }
    for (unsigned i = 0; i < constraint->outputSize (); ++i)
      {
	boost::shared_ptr<Split<C> > split (new Split<C> (constraint, i));
	if (scale.empty ())
	  problem.addConstraint (split, interval[i]);
	else
	  problem.addConstraint (split, interval[i], scale[i]);
      }
  }
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FILTER_SPLIT_HXX
