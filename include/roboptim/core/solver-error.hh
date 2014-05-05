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

#ifndef ROBOPTIM_CORE_SOLVER_ERROR_HH
# define ROBOPTIM_CORE_SOLVER_ERROR_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <iostream>
# include <stdexcept>
# include <boost/optional.hpp>

# include <roboptim/core/result.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Base exception class for solving errors.
  /// All other exceptions classes concerning the optimization
  /// process should inherits this class.
  class ROBOPTIM_DLLAPI SolverError : public std::runtime_error
  {
  public:
    /// \brief Instantiate an error from an error message.
    /// \param arg error message.
    explicit SolverError (const std::string& arg);

    /// \brief Instantiate an error from an error message.
    /// \param arg error message.
    /// \param res last state of the solver.
    SolverError (const std::string& arg,
                 const Result& res);

    /// \brief Copy constructor.
    /// \param error other error to copy.
    SolverError (const SolverError& error);

    /// \brief Trivial destructor.
    ~SolverError() throw ();

    /// \brief Display the error on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;

    /// \brief Retrieve the (optional) last state of the solver.
    /// \return last state of the solver.
    const boost::optional<Result>& lastState () const;

    /// \brief Retrieve the (optional) last state of the solver.
    /// \return last state of the solver.
    boost::optional<Result>& lastState ();

  private:
    /// \brief (Optional) Last state of the solver before the error was raised.
    ///
    /// \warning This piece of data may be completely unusable, and comes
    /// without any warranty of any kind.
    boost::optional<Result> lastState_;
  };

  /// @}

  /// \brief Override operator<< to handle error display.
  ///
  /// \param o output stream used for display
  /// \param e error to be displayed
  /// \return output stream
  ROBOPTIM_DLLAPI std::ostream& operator<< (std::ostream& o,
					    const SolverError& e);
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_ERROR_HH
