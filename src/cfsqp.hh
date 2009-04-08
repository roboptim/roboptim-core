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
  class CFSQPSolver : public Solver<DerivableFunction, DerivableFunction>
  {
  public:
    typedef Solver<DerivableFunction, DerivableFunction> parent_t;

    /// Constructor.
    explicit CFSQPSolver (const problem_t&, int = 0) throw ();
    /// Destructor.
    virtual ~CFSQPSolver () throw ();
    /// Solve the problem.
    virtual void solve () throw ();
  private:
    /// Initialize bounds.
    void initialize_bounds (double* bl, double* bu) const throw ();

    /// Logging level.
    int iprint_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_CFSQP_HH
