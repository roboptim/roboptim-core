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

#ifndef ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
# define ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
# include <limits>
# include <utility>

# include <boost/numeric/ublas/symmetric.hpp>

# include <roboptim/core/derivable-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_meta_function
  /// @{

  /// \brief Define an abstract function which is derivable twice (\f$C^2\f$).
  ///
  /// A twice derivable function is a derivable function
  /// which provides a way to compute its hessian.
  ///
  /// \f[ f : x \rightarrow f(x) \f]
  /// \f$x \in \mathbb{R}^n\f$, \f$f(x) \in \mathbb{R}^m\f$ where
  /// \f$n\f$ is the input size and \f$m\f$ is the output size.
  ///
  /// Hessian computation is done through the #impl_hessian method
  /// that has to implemented by the concrete class inheriting this
  /// class.
  ///
  /// The hessian of a \f$\mathbb{R}^n \rightarrow \mathbb{R}^m\f$
  /// function where \f$n > 1\f$ and \f$m > 1\f$ is a tensor.
  /// To avoid this costly representation, the function is split
  /// into \f$m\f$ \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$ functions.
  /// See #DerivableFunction documentation for more information.
  class TwiceDerivableFunction : public DerivableFunction
  {
  public:
    /// \brief Hessian type.
    ///
    /// Hessians are symmetric matrices.
    typedef ublas::symmetric_matrix<value_type, ublas::lower> hessian_t;

    /// \brief Hessian size type represented as a pair of values.
    typedef std::pair<value_type, value_type> hessianSize_t;


    /// \brief Return the size of a hessian.
    ///
    /// Hessian size is equales to (input size, input size).
    /// \return hessian's size as a pair
    hessianSize_t hessianSize () const throw ()
    {
      return std::make_pair (inputSize (), inputSize ());
    }

    /// \brief Check if the hessian is valid (check sizes).
    ///
    /// \param hessian hessian that will be checked
    /// \return true if valid, false if not
    bool isValidHessian (const hessian_t& hessian) const throw ()
    {
      return hessian.size1 () == this->hessianSize ().first
	&& hessian.size2 () == this->hessianSize ().second;
    }


    /// \brief Compute the hessian at a given point.
    ///
    /// Program will abort if the argument size is wrong.
    /// \param argument point where the hessian will be computed
    /// \param functionId evaluated function id in the split representation
    /// \return computed hessian
    hessian_t hessian (const argument_t& argument,
		       size_type functionId = 0) const throw ()
    {
      hessian_t hessian (hessianSize ().first, hessianSize ().second);
      hessian.clear ();
      this->hessian (hessian, argument, functionId);
      return hessian;
    }

    /// \brief Compute the hessian at a given point.
    ///
    /// Program will abort if the argument size is wrong.
    /// \param hessian hessian will be stored here
    /// \param argument point where the hessian will be computed
    /// \param functionId evaluated function id in the split representation
    void hessian (hessian_t& hessian,
		  const argument_t& argument,
		  size_type functionId = 0) const throw ()
    {
      assert (isValidHessian (hessian));
      this->impl_hessian (hessian, argument, functionId);
      assert (isValidHessian (hessian));
    }


    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    /// \brief Concrete class constructor should call this constructor.
    ///
    /// \param inputSize input size (argument size)
    /// \param outputSize output size (result size)
    /// \param name function's name
    TwiceDerivableFunction (size_type inputSize,
			    size_type outputSize = 1,
			    std::string name = std::string ()) throw ();

    /// \brief Hessian evaluation.
    ///
    /// Compute the hessian, has to be implemented in concrete classes.
    /// The hessian is computed for a specific sub-function which id is
    /// passed through the functionId argument.
    /// \warning Do not call this function directly, call #hessian instead.
    /// \param hessian hessian will be stored here
    /// \param argument point where the hessian will be computed
    /// \param functionId evaluated function id in the split representation
    virtual void impl_hessian (hessian_t& hessian,
			       const argument_t& argument,
			       size_type functionId = 0) const throw () = 0;
  };

  /// @}
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_TWICE_DERIVABLE_FUNCTION_HH
