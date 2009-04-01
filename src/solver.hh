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
 * \file src/solver.hh
 *
 * \brief Declaration of the Solver class.
 */

#ifndef OPTIMIZATION_SOLVER_HH
# define OPTIMIZATION_SOLVER_HH
# include <stdexcept>
# include <boost/utility.hpp>
# include <boost/variant/variant.hpp>

# include <optimization-fwd.hh>
# include <function.hh>

namespace optimization
{
  /// \brief Base error.
  /// All other errors inherits this class.
  struct SolverError : public std::runtime_error
  {
    SolverError (const std::string& arg) throw ()
      : std::runtime_error (arg)
    {}
  };

  /// \brief Returned by Solver::getMinimum if no solution can be
  /// found (but no error has been encountered during the process).
  struct NoSolution {};

  /// Generic solver class.
  class Solver : public boost::noncopyable
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


     /// \defgroup Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit Solver (const Problem&) throw ();

    /// Destructor.
    virtual ~Solver () throw ();
    /// \}

    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum is called.
    void reset () throw ();

    /// Returns the function minimum (and solve the problem, if
    /// it has not yet been solved).
    virtual result_t getMinimum () throw () = 0;

    /// Problem which has to be solved.
    const Problem& problem;

  protected:
    /// Result of minimization.
    result_t result_;
  };

} // end of namespace optimization

#endif //! OPTIMIZATION_SOLVER_HH
