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
 * \brief Declaration of the TwiceDerivableFunction class.
 */

#ifndef ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
# define ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
# include <limits>
# include <utility>

# include <boost/numeric/ublas/symmetric.hpp>

# include <roboptim/core/derivable-function.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_function
     @{
  */

  /// Define a function which is derivable twice.
  class TwiceDerivableFunction : public DerivableFunction
  {
  public:
    /// Hessian type.
    typedef ublas::symmetric_matrix<value_type, ublas::lower> hessian_t;

    /// Hessian size type.
    typedef std::pair<value_type, value_type> hessianSize_t;


    /// Return the size of a hessian.
    hessianSize_t hessianSize () const throw ()
    {
      return std::make_pair (inputSize (), inputSize ());
    }

    /// Check if a hessian is valid (check sizes).
    bool isValidHessian (const hessian_t& hessian) const throw ()
    {
      return hessian.size1 () == this->hessianSize ().first
	&& hessian.size2 () == this->hessianSize ().second;
    }


    /// Compute the hessian.
    hessian_t hessian (const argument_t& argument,
		       int functionId) const throw ()
    {
      hessian_t hessian (hessianSize ().first, hessianSize ().second);
      hessian.clear ();
      this->hessian (hessian, argument, functionId);
      return hessian;
    }

    /// Compute the hessian.
    void hessian (hessian_t& hessian,
		  const argument_t& argument,
		  int functionId = 0) const throw ()
    {
      assert (isValidHessian (hessian));
      this->impl_hessian (hessian, argument, functionId);
      assert (isValidHessian (hessian));
    }


    /// Display the function.
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    /// Constructor.
    TwiceDerivableFunction (size_type inputSize,
			    size_type outputSize = 1) throw ();

    /// Hessian.
    virtual void impl_hessian (hessian_t& hessian,
			       const argument_t& argument,
			       int functionId = 0) const throw () = 0;
  };

  /**
     @}
  */

} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
