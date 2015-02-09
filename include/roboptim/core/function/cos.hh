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

#ifndef ROBOPTIM_CORE_FUNCTION_COS_HH
# define ROBOPTIM_CORE_FUNCTION_COS_HH
# include <roboptim/core/fwd.hh>
# include <roboptim/core/twice-differentiable-function.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Cos function.
  template <typename T>
  class Cos : public GenericTwiceDifferentiableFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericTwiceDifferentiableFunction<T>);

    /// \brief Build a cosinus function.
    ///
    /// \param offset cosinus function offset
    Cos ()
      : GenericTwiceDifferentiableFunction<T>
	(1, 1, "cos")
    {}

    ~Cos ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const
    {
      return o << "Cos function";
    }

  protected:
    void impl_compute (result_ref result, const_argument_ref x) const
    {
      result[0] = std::cos (x[0]);
    }

    void impl_gradient (gradient_ref gradient, const_argument_ref x, size_type)
    const;

    void impl_jacobian (jacobian_ref jacobian, const_argument_ref x) const;

    void impl_hessian
    (hessian_ref hessian, const_argument_ref x, size_type) const;

  };

  template <>
  inline void
  Cos<EigenMatrixSparse>::impl_gradient (gradient_ref gradient, const_argument_ref x, size_type)
    const
  {
    gradient.insert (0) = -std::sin (x[0]);
  }

  template <typename T>
  void
  Cos<T>::impl_gradient (gradient_ref gradient, const_argument_ref x, size_type)
    const
  {
    gradient[0] = -std::sin (x[0]);
  }

  template <>
  inline void
  Cos<EigenMatrixSparse>::impl_jacobian
  (jacobian_ref jacobian, const_argument_ref x) const
  {
    jacobian.coeffRef (0, 0) = -std::sin (x[0]);
  }
  template <typename T>
  void
  Cos<T>::impl_jacobian
  (jacobian_ref jacobian, const_argument_ref x) const
  {
    jacobian (0, 0) = -std::sin (x[0]);
  }

  template <typename T>
  void
  Cos<T>::impl_hessian (hessian_ref hessian,
			const_argument_ref x,
			size_type) const
  {
    hessian (0, 0) = -std::cos (x[0]);
  }

  template <>
  inline void
  Cos<EigenMatrixSparse>::impl_hessian (hessian_ref hessian,
					const_argument_ref x,
					size_type) const
  {
    hessian.coeffRef (0, 0) = -std::cos (x[0]);
  }


  /// Example shows cosinus function use.
  /// \example function_cos.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_COS_HH
