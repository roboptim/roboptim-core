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
 * \brief Declaration of the GenericSolver class.
 */

#ifndef OPTIMIZATION_GENERIC_SOLVER_HH
# define OPTIMIZATION_GENERIC_SOLVER_HH
# include <stdexcept>
# include <boost/utility.hpp>
# include <boost/variant/variant.hpp>

# include <liboptimization/fwd.hh>
# include <liboptimization/problem.hh>
# include <liboptimization/solver-error.hh>

namespace optimization
{
  /// Abstract interface satisfied by all solvers.
  class GenericSolver : public boost::noncopyable
  {
  public:
    /// Define the kind of solution which has been found.
    enum solutions {
      /// Solution has yet to be found.
      SOLVER_NO_SOLUTION,
      /// Solution has been found.
      SOLVER_VALUE,
      /// The solver failed to found a solution.
      SOLVER_ERROR
    };

    /// Vector type.
    typedef Function::vector_t vector_t;

    /// Result type.
    typedef boost::variant<NoSolution, vector_t, SolverError> result_t;

     /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    explicit GenericSolver () throw ();

    /// Destructor.
    virtual ~GenericSolver () throw ();
    /// \}

    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum is called.
    void reset () throw ();

    /// Solve the problem.
    /// Called automatically by getMinimum if required.
    virtual void solve () throw () = 0;

    /// Returns the function minimum (and solve the problem, if
    /// it has not yet been solved).
    const result_t& getMinimum () throw ();
  protected:
    /// Result of minimization.
    result_t result_;
  };

}; // end of namespace optimization

#endif //! OPTIMIZATION_GENERIC_SOLVER_HH
