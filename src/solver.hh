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

# include <fwd.hh>
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

  /// Solver for linear problems.
  class LinearSolver : public boost::noncopyable
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

    /// Function type.
    typedef LinearFunction function_t;


     /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit LinearSolver (const Function&) throw ();

    /// Destructor.
    virtual ~LinearSolver () throw ();
    /// \}

    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum is called.
    void reset () throw ();

    /// Returns the function that has to be minimized.
    const function_t& getFunction () const throw ();

    /// Returns the function minimum (and solve the problem, if
    /// it has not yet been solved).
    virtual result_t getMinimum () throw () = 0;

    /// Add a linear constraint.
    virtual void addLinearConstraint (const function_t&) throw () = 0;

    /// Starting point.
    vector_t start;

  protected:
    /// Result of minimization.
    result_t result_;

  private:
    // TODO: shouldn't be a pointer as problem might be modified,
    // behind solver's back.
    /// Cost function.
    const Function& function_;
  };


  /// Solve Quadratic problems.
  class QuadraticSolver : public LinearSolver
  {
  public:
    /// Function type.
    typedef QuadraticFunction function_t;

    /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit QuadraticSolver (const Function&) throw ();

    /// Destructor.
    virtual ~QuadraticSolver () throw ();
    /// \}

    /// Returns the function that has to be minimized.
    const function_t& getFunction () const throw ();

    /// Add a quadratic constraint.
    virtual void addQuadraticConstraint (const function_t&) throw () = 0;
  };

  /// Solve C2 problems.
  class C2Solver : public QuadraticSolver
  {
  public:
    /// Function type.
    typedef TwiceDerivableFunction function_t;

    /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit C2Solver (const Function&) throw ();

    /// Destructor.
    virtual ~C2Solver () throw ();
    /// \}

    /// Returns the function that has to be minimized.
    const function_t& getFunction () const throw ();

    /// Add a C2 constraint.
    virtual void addC2Constraint (const function_t&) throw () = 0;

  };

  /// Solve C1 problems.
  class C1Solver : public C2Solver
  {
  public:
    /// Function type.
    typedef DerivableFunction function_t;

    /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit C1Solver (const Function&) throw ();

    /// Destructor.
    virtual ~C1Solver () throw ();
    /// \}

    /// Returns the function that has to be minimized.
    const function_t& getFunction () const throw ();

    /// Add a C1 constraint.
    virtual void addC1Constraint (const function_t&) throw () = 0;

  };

  /// Solve C0 problems.
  class C0Solver : public C1Solver
  {
  public:
    /// Function type.
    typedef Function function_t;

    /// \defgroup ctor Constructors and destructors.
    /// \{
    /// Main constructor.
    /// \param pb Problem.
    explicit C0Solver (const Function&) throw ();

    /// Destructor.
    virtual ~C0Solver () throw ();
    /// \}

    /// Returns the function that has to be minimized.
    const function_t& getFunction () const throw ();

    /// Add a C0 constraint.
    virtual void addC0Constraint (const function_t&) throw () = 0;

  };

} // end of namespace optimization

#endif //! OPTIMIZATION_SOLVER_HH
