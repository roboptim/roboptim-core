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
 * \file src/cfsqp.hh
 *
 * \brief Implementation of the CFSQP module.
 */

#ifndef OPTIMIZATION_CFSQP_HH
# define OPTIMIZATION_CFSQP_HH
# include <string>
# include <solver.hh>

namespace optimization
{
  /// CFSQP solver.
  class CFSQPSolver : public C1Solver
  {
  public:
    /// Constructor.
    explicit CFSQPSolver (const TwiceDerivableFunction&, int = 0) throw ();
    /// Destructor.
    virtual ~CFSQPSolver () throw ();
    /// Return the default solver error.
    virtual result_t getMinimum () throw ();

    virtual void addLinearConstraint (const LinearFunction&) throw ();
    virtual void addQuadraticConstraint (const QuadraticFunction&) throw ();
    virtual void addC1Constraint (const DerivableFunction&) throw ();
    virtual void addC2Constraint (const TwiceDerivableFunction&) throw ();

    //FIXME: make private.
    std::vector<const DerivableFunction*> constraints;
  private:
    /// Initialize bounds.
    void initialize_bounds (double* bl, double* bu) const throw ();

    /// Logging level.
    int iprint_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_CFSQP_HH
