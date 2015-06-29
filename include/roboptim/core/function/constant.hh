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

#ifndef ROBOPTIM_CORE_FUNCTION_CONSTANT_HH
# define ROBOPTIM_CORE_FUNCTION_CONSTANT_HH
# include <roboptim/core/fwd.hh>
# include <roboptim/core/linear-function.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  /// \addtogroup roboptim_function
  /// @{

  /// \brief Constant function.
  ///
  /// Implement a constant function using the formula:
  /// \f[f(x) = offset\f]
  /// where \f$offset\f$ is set when the class is instantiated.
  template <typename T>
  class GenericConstantFunction : public GenericLinearFunction<T>
  {
  public:
    ROBOPTIM_TWICE_DIFFERENTIABLE_FUNCTION_FWD_TYPEDEFS_
    (GenericLinearFunction<T>);

    /// \brief Build a constant function.
    ///
    /// \param offset constant function offset
    GenericConstantFunction (const_vector_ref offset)
      : GenericLinearFunction<T> (static_cast<size_type> (offset.size ()),
				  static_cast<size_type> (offset.size ()),
				  "constant function"),
	offset_ (offset)
    {
    }

    /// \brief Build a constant function.
    ///
    /// \param input_size input size of the function
    /// \param offset constant function offset
    GenericConstantFunction (size_type input_size, const_vector_ref offset)
      : GenericLinearFunction<T> (input_size,
				  static_cast<size_type> (offset.size ()),
				  "constant function"),
	offset_ (offset)
    {
    }

    ~GenericConstantFunction ()
    {}

    /// \brief Display the function on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const
    {
      return o << "Constant function" << incindent << iendl
	       << "offset = " << this->offset_ << decindent;
    }

  protected:
    void impl_compute (result_ref result, const_argument_ref) const
    {
      result = this->offset_;
    }

    void impl_gradient (gradient_ref gradient, const_argument_ref, size_type = 0)
      const
    {
      gradient.setZero ();
    }

    void impl_jacobian (jacobian_ref jacobian, const_argument_ref) const
    {
      jacobian.setZero ();
    }

  public:
    /// \brief Get the type-checking flag
    virtual unsigned long flag() const
    {
      return flag_;
    }

    /// \brief Flag representing the Roboptim Function type
    static const unsigned long flag_;

  private:
    const vector_t offset_;
  };

  /// Example shows constant function use.
  /// \example constant-function.cc

  /// @}

  template <typename T>
  const unsigned long GenericConstantFunction<T>::flag_ = ROBOPTIM_IS_CONSTANT|GenericConstantFunction<T>::parent_t::flag_;

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FUNCTION_CONSTANT_HH
