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
# include <solver.hh>

namespace optimization
{
  /// CFSQP solver.
  class CFSQPSolver : public Solver
  {
  public:
    /// Constructor.
    explicit CFSQPSolver (const Problem&) throw ();
    /// Destructor.
    virtual ~CFSQPSolver () throw ();
    /// Return the default solver error.
    virtual result_t getMinimum () throw ();

  private:
    /// Initialize bounds.
    void initialize_bounds (double* bl, double* bu) const throw ();
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_CFSQP_HH
