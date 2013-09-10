// Copyright (C) 2013 by Thomas Moulard, AIST, CNRS, INRIA.
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

#ifndef ROBOPTIM_CORE_FILTER_DERIVATIVE_HH
# define ROBOPTIM_CORE_FILTER_DERIVATIVE_HH
# include <cassert>
# include <vector>

# include <boost/make_shared.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/type_traits/is_base_of.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \brief Return the derivative of a function.
  ///
  /// Know issue: for now it returns a non-differentiable function.
  template <typename U>
  class Derivative;

  namespace
  {
    template <typename U>
    struct DerivativeParent
    {
      typedef typename boost::mpl::if_<
	boost::is_base_of<GenericTwiceDifferentiableFunction<typename U::traits_t>, U>,
	GenericDifferentiableFunction<typename U::traits_t>,
	GenericFunction<typename U::traits_t>
	>::type result_t;
    };
  } // end of anonymous namespace.

  template <typename U>
  class Derivative : public DerivativeParent<U>::result_t
  {
  public:
    ROBOPTIM_FUNCTION_FWD_TYPEDEFS_
    (typename DerivativeParent<U>::result_t);

    /// \brief Gradient type.
    typedef typename GenericFunctionTraits<typename U::traits_t>::gradient_t
    gradient_t;
    /// \brief Jacobian type.
    typedef typename GenericFunctionTraits<typename U::traits_t>::jacobian_t
    jacobian_t;
    /// \brief Jacobian size type (pair of values).
    typedef std::pair<size_type, size_type> jacobianSize_t;


    typedef boost::shared_ptr<Derivative> DerivativeShPtr_t;

    explicit Derivative (boost::shared_ptr<U> origin,
			size_type variableId) throw ()
      : DerivativeParent<U>::result_t
	(origin->inputSize (),
	 origin->outputSize (),
	 "derivative of " + origin->getName ()),
	origin_ (origin),
	variableId_ (variableId),
	jacobian_ (origin->outputSize (),
		   origin->inputSize ()),
	hessian_ (origin->inputSize (),
		  origin->inputSize ())
    {
      assert (variableId_ < this->inputSize ());
    }

    ~Derivative () throw ()
    {}

    const boost::shared_ptr<U>& origin () const
    {
      return origin_;
    }

    boost::shared_ptr<U>& origin ()
    {
      return origin_;
    }

  protected:
    // FIXME: this is inefficient.
    void impl_compute (result_t& result, const argument_t& x)
      const throw ()
    {
      origin_->jacobian (jacobian_, x);
      result = jacobian_.block (0, variableId_, this->outputSize (), 1);
    }

    void impl_gradient (gradient_t& gradient,
			const argument_t& x,
			size_type functionId = 0)
      const throw ()
    {
      origin_->hessian (hessian_, x, functionId);
      for (size_type i = 0; i < this->inputSize (); ++i)
	gradient.coeffRef (i) = hessian_.coeffRef (i, i);
    }

  private:
    boost::shared_ptr<U> origin_;
    size_type variableId_;
    mutable matrix_t jacobian_;
    mutable matrix_t hessian_;
  };

  template <typename U>
  boost::shared_ptr<Derivative<U> >
  derivative (boost::shared_ptr<U> origin,
	     typename Derivative<U>::size_type variableId = 0)
  {
    return boost::make_shared<Derivative<U> > (origin, variableId);
  }

} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_FILTER_DERIVATIVE_HH
