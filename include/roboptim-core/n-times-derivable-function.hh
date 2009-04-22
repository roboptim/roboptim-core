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
 * \brief Class NTimesDerivable declaration.
 */

#ifndef ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# define ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
# include <roboptim-core/twice-derivable-function.hh>

namespace roboptim
{
  /// \brief Define a \f$\mathbb{R} \rightarrow \mathbb{R}^m\f$ function,
  /// derivable n times (n > 2).
  template <unsigned DerivabilityOrder>
  class NTimesDerivableFunction : public TwiceDerivableFunction
  {
  public:
    static const size_type derivabilityOrder = DerivabilityOrder;

    NTimesDerivableFunction (size_type m = 1) throw ();
    virtual ~NTimesDerivableFunction () throw ();

    virtual vector_t operator () (const vector_t&) const throw ();
    virtual vector_t operator () (double) const throw () = 0;

    virtual gradient_t gradient (const vector_t&, int = 0) const throw ();
    virtual hessian_t hessian (const vector_t&, int) const throw ();

    virtual vector_t derivative (double x, size_type order = 1) const
      throw () = 0;

    virtual std::ostream& print (std::ostream&) const throw ();
  };
}

# include <roboptim-core/n-times-derivable-function.hxx>
#endif //! ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
