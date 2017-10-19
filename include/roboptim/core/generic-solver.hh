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

#ifndef ROBOPTIM_CORE_GENERIC_SOLVER_HH
# define ROBOPTIM_CORE_GENERIC_SOLVER_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <stdexcept>

// Fix bug in Boost 1.58.
// cf. https://svn.boost.org/trac/boost/ticket/11199
# include <boost/type_traits/remove_cv.hpp>

# include <boost/variant/get.hpp>
# include <boost/variant/variant.hpp>
# include <boost/utility.hpp>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/problem.hh>
# include <roboptim/core/result.hh>
# include <roboptim/core/solver-error.hh>
# include <roboptim/core/solver-warning.hh>

// Kept for backward compatibility until ResultWithWarnings is removed
# include <roboptim/core/deprecated.hh>
# include <roboptim/core/result-with-warnings.hh>

namespace roboptim
{
  /// \addtogroup roboptim_solver
  /// @{

  /// \brief Abstract interface satisfied by all solvers.
  class ROBOPTIM_CORE_DLLAPI GenericSolver : public boost::noncopyable
  {
  public:
    /// \brief Define the kind of solution which has been found.
    enum solutions {
      /// Solution has yet to be found.
      SOLVER_NO_SOLUTION,
      /// Solution has been found.
      SOLVER_VALUE,
      /// The solver failed to find a solution.
      SOLVER_ERROR,
      /// Solution has been found but some problems happened.
      SOLVER_VALUE_WARNINGS
    };

    /// \brief Result type.
    ///
    /// Uses a Boost.Variant to represent the different possible results:
    /// - no solution (problem not yet solved),
    /// - result (problem has been solved successfully, possibly with warnings),
    /// - solver error (optimization has failed).
    /// Note: the usage of ResultWithWarnings is deprecated. Use Result
    /// directly.
    ROBOPTIM_ALLOW_DEPRECATED_ON
    typedef boost::variant<NoSolution,
                           Result,
                           SolverError,
                           ResultWithWarnings> result_t;
    ROBOPTIM_ALLOW_DEPRECATED_OFF

    /// \name Constructors and destructors.
    /// \{
    explicit GenericSolver ();
    explicit GenericSolver (const GenericSolver&);
    virtual ~GenericSolver ();
    /// \}

    /// \brief Force to restart the optimization.
    ///
    /// Reset the internal mechanism to force the solution to be
    /// re-computed next time getMinimum() is called.
    void reset ();

    /// \brief Solve the problem.
    ///
    /// Called automatically by getMinimum() if required.
    virtual void solve () = 0;

    /// \brief Returns the function minimum.
    ///
    /// This solves the problem automatically, if it has not yet been solved.
    /// \see minimumType()
    /// \see getMinimum()
    const result_t& minimum ();

    /// \brief Display the solver on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;


    /// \brief Determine real minimum type.
    ///
    /// \return value representing result type
    solutions minimumType ()
    {
      switch (minimum ().which ())
	{
	case 0:
	  return SOLVER_NO_SOLUTION;
	case 1:
	  return SOLVER_VALUE;
	case 2:
	  return SOLVER_ERROR;
	default:
	  break;
	}
      assert (0);
      return SOLVER_ERROR;
    }

    /// \brief Get real result.
    ///
    /// Optimization results is wrapped in a Boost.Variant
    /// class, this method has to be used to retrieve the
    /// real result type.
    /// \return real result
    /// \throw boost::bad_get if a wrong type is used.
    template <typename T>
    const T& getMinimum ()
    {
      return boost::get<T> (minimum ());
    }

  protected:
    /// \brief Optimization result.
    result_t result_;
  };

  /// @}

  /// \brief Override operator<< to handle solver display.
  ///
  /// \param o output stream used for display
  /// \param gs solver to be displayed
  /// \return output stream
  ROBOPTIM_CORE_DLLAPI std::ostream& operator<< (std::ostream& o,
					    const GenericSolver& gs);


  /// \brief Override operator<< to display ``no solution'' objects.
  ///
  /// \param o output stream used for display
  /// \param ns NoSolution object, ignored
  /// \return output stream
  ROBOPTIM_CORE_DLLAPI std::ostream& operator<< (std::ostream& o,
					    const NoSolution& ns);

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_GENERIC_SOLVER_HH
