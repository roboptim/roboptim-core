// Copyright (C) 2009 by Thomas Moulard, FIXME.
//
// This file is part of the liboptimization.
//
// liboptimization is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// liboptimization is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with liboptimization.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file src/dummy.hh
 *
 * \brief Implementation of the dummy module (always fail).
 */

#ifndef OPTIMIZATION_DUMMY_HH
# define OPTIMIZATION_DUMMY_HH
# include <solver.hh>

namespace optimization
{
  /// Dummy solver which always fails.
  class DummySolver : public C0Solver
  {
  public:
    /// Constructor.
    explicit DummySolver (const Function&) throw ();
    /// Destructor.
    virtual ~DummySolver () throw ();
    /// Return the default solver error.
    virtual result_t getMinimum () throw ();

    virtual void addLinearConstraint (const LinearFunction&) throw ();
    virtual void addQuadraticConstraint (const QuadraticFunction&) throw ();
    virtual void addC2Constraint (const TwiceDerivableFunction&) throw ();
    virtual void addC1Constraint (const DerivableFunction&) throw ();
    virtual void addC0Constraint (const Function&) throw ();
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_DUMMY_HH
