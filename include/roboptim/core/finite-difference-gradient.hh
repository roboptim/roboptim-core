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
 * \brief Declaration of the FiniteDifferenceGradient class.
 */

#ifndef ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# define ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# include <roboptim/core/fwd.hh>

# include <roboptim/core/derivable-function.hh>

namespace roboptim
{
  /// \brief Compute automatically a gradient with finite differences.
  class FiniteDifferenceGradient : public DerivableFunction
  {
  public:
    FiniteDifferenceGradient (const Function&, value_type = 1e-4) throw ();
    ~FiniteDifferenceGradient () throw ();

  protected:
    void impl_compute (result_t&, const argument_t&) const throw ();
    void impl_gradient (gradient_t&, const argument_t&, int) const throw ();

    const Function& adaptee_;
    const value_type epsilon_;
  };

  /// Check if a gradient is valid by comparing the distance between its
  /// gradient and an automatically computed finite difference gradient.
  bool checkGradient (DerivableFunction&,
		      int,
		      const Function::vector_t&,
		      Function::value_type = .001) throw ();
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
