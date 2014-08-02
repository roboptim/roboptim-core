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

#ifndef ROBOPTIM_CORE_FUNCTION_IDENTITY_HH
# define ROBOPTIM_CORE_FUNCTION_IDENTITY_HH
# include <roboptim/core/twice-differentiable-function.hh>
# include <roboptim/core/linear-function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Identity function.
  ///
  /// Implement a linear function using the formula:
  /// \f[f(x) = x + offset\f]
  /// where \f$A\f$ and \f$b\f$ are set when the class is instantiated.
  template <typename T>
  class GenericIdentityFunction : public GenericLinearFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericLinearFunction<T>);

    /// \brief Build an identity function.
    ///
    /// \param offset identity function offset
    GenericIdentityFunction (const vector_t& offset)
      : GenericLinearFunction<T> (static_cast<size_type> (offset.size ()),
				  static_cast<size_type> (offset.size ())),
	offset_ (offset)
    {}

    ~GenericIdentityFunction ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const
    {
      return o << "Identity function" << incindent << iendl
	       << "offset = " << this->offset_
	       << decindent;
    }

  protected:

    void impl_compute (result_t& result,
		       const argument_t& argument)
      const
    {
      result = argument + this->offset_;
    }

    void
    impl_jacobian (jacobian_t& jacobian,
		   const argument_t&) const
    {
      jacobian.setIdentity ();
    }

    void
    impl_gradient (gradient_t& gradient,
		   const argument_t& ,
		   size_type idFunction) const;

  private:
    vector_t offset_;
  };

  template <>
  inline void
  GenericIdentityFunction<EigenMatrixSparse>::impl_gradient
  (gradient_t& gradient, const argument_t&, size_type idFunction) const
  {
    gradient.setZero ();
    gradient.insert (idFunction) = 1.;
  }

  template <typename T>
  void
  GenericIdentityFunction<T>::impl_gradient
  (gradient_t& gradient, const argument_t&, size_type idFunction) const
  {
    gradient.setZero ();
    gradient[idFunction] = 1.;
  }

  /// Example shows identity function use.
  /// \example identity-function.cc

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_IDENTITY_FUNCTION_HH
