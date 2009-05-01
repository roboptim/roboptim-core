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
# include <boost/static_assert.hpp>

# include <roboptim-core/twice-derivable-function.hh>

namespace roboptim
{
  /**
     \addtogroup roboptim_function
     @{
  */

  template <unsigned dorder>
  class NTimesDerivableFunction;

  template <>
  class NTimesDerivableFunction<2> : public TwiceDerivableFunction
  {
  public:
    static const size_type derivabilityOrder = 2;

    NTimesDerivableFunction (size_type m = 1) throw ()
      : TwiceDerivableFunction (1, m)
    {}

    virtual ~NTimesDerivableFunction () throw ()
    {}

    virtual vector_t operator () (const vector_t& x) const throw ()
    {
      assert (x.size () == 1);
      return operator () (x[0]);
    }

    virtual vector_t operator () (double) const throw () = 0;

    virtual gradient_t gradient (const vector_t& x, int i = 0) const throw ()
    {
      assert (x.size () == 1);
      vector_t d = derivative (x[0], 1);

      vector_t res (1);
      res[0] = d[i];
      return res;
    }

    virtual hessian_t hessian (const vector_t& x, int i) const throw ()
    {
      hessian_t hessian (1, 1);

      vector_t dd = derivative (x[0], 2);
      hessian (0, 0) = dd[i];

      return hessian;
    }

    virtual vector_t derivative (double x, size_type order = 1) const
      throw () = 0;

    virtual std::ostream& print (std::ostream& o) const throw ()
    {
      o << "Function derivable " << derivabilityOrder << " times.";
      return o;
    }

  };

  /// \brief Define a \f$\mathbb{R} \rightarrow \mathbb{R}^m\f$ function,
  /// derivable n times (n >= 2).
  template <unsigned DerivabilityOrder>
  class NTimesDerivableFunction
    : public NTimesDerivableFunction<DerivabilityOrder - 1>
  {
  public:
    BOOST_STATIC_ASSERT(DerivabilityOrder > 2);

    typedef Function::size_type size_type;
    static const size_type derivabilityOrder = DerivabilityOrder;

    NTimesDerivableFunction (size_type m = 1) throw ()
    : NTimesDerivableFunction<DerivabilityOrder - 1> (m)
    {
    }

    virtual ~NTimesDerivableFunction () throw ();

    virtual std::ostream& print (std::ostream&) const throw ();
  };

  /**
     @}
  */
}
# include <roboptim-core/n-times-derivable-function.hxx>
#endif //! ROBOPTIM_TRAJECTORY_N_TIMES_DERIVABLE_HH
