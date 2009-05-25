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
 * \brief Declaration of the NumericLinearFunction class.
 */

#ifndef ROBOPTIM_CORE_NUMERIC_LINEAR_FUNCTION_HH
# define ROBOPTIM_CORE_NUMERIC_LINEAR_FUNCTION_HH

# include <roboptim/core/linear-function.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_function
     @{
  */

  /// Define a quadratic function.
  class NumericLinearFunction : public LinearFunction
  {
  public:
    NumericLinearFunction (const matrix_t&, const vector_t&) throw ();
    ~NumericLinearFunction () throw ();

    virtual std::ostream& print (std::ostream&) const throw ();

  protected:
    void impl_compute (result_t& , const vector_t&) const throw ();
    void impl_gradient (gradient_t&, const vector_t&, int = 0) const throw ();
    void impl_jacobian (jacobian_t&, const vector_t&) const throw ();

  private:
    matrix_t a_;
    vector_t b_;
  };
  /**
     @}
  */
} // end of namespace roboptim
#endif //! ROBOPTIM_CORE_QUADRATIC_FUNCTION_HH
