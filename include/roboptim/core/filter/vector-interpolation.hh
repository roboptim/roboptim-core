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

#ifndef ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HH
# define ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HH
# include <vector>
# include <boost/shared_ptr.hpp>

# include <roboptim/core/detail/autopromote.hh>
# include <roboptim/core/differentiable-function.hh>


namespace roboptim
{
  /// \brief Takes a vector or argument and differentiate it.
  ///
  /// It is common to have an optimization vector where a pattern is
  /// repeated:
  ///
  /// [x_0^0 ... x_N^0 ... x_0^M x_N^M]
  ///
  /// The pattern is here of length N and repeated M times.
  ///
  /// By using numerical interpolation, it is possible to
  /// differentiate this pattern.
  ///
  /// A classical example are trajectories.
  template <typename T>
  class VectorInterpolation : public GenericDifferentiableFunction<T>
  {
  public:
    ROBOPTIM_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericDifferentiableFunction<T>);

    typedef boost::shared_ptr<VectorInterpolation> VectorInterpolationShPtr_t;

    explicit VectorInterpolation
    (const vector_t& x, size_type outputSize, value_type dt)
      throw ();
    ~VectorInterpolation () throw ();

    void impl_compute (result_t& result, const argument_t& x)
      const throw ();

    void impl_gradient (gradient_t& gradient,
			const argument_t& argument,
			size_type functionId = 0)
      const throw ();
    void impl_jacobian (jacobian_t& jacobian,
			const argument_t& arg)
      const throw ();
  private:
    vector_t x_;
    vector_t dx_;
    value_type dt_;
  };

  template <typename T>
  boost::shared_ptr<VectorInterpolation<T> >
  vectorInterpolation (typename VectorInterpolation<T>::vector_t x,
		       typename VectorInterpolation<T>::size_type outputSize,
		       typename VectorInterpolation<T>::value_type dt = 1.)
  {
    return boost::make_shared<VectorInterpolation<T> > (x, outputSize, dt);
  }

} // end of namespace roboptim.

# include <roboptim/core/filter/vector-interpolation.hxx>
#endif //! ROBOPTIM_CORE_FILTER_VECTOR_INTERPOLATION_HH
