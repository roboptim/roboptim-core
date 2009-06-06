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
# include <iostream>
# include <stdexcept>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Base exception class for solving errors.
  /// All other exceptions classes concerning the optimization
  /// process should inherits this class.
  class SolverError : public std::runtime_error
  {
  public:
    /// \brief Instantiate an error from an error message.
    /// \param arg error message.
    explicit SolverError (const std::string& arg) throw ();

    /// \brief Display the error on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const throw ();
  };

  /// @}

  /// \brief Override operator<< to handle error display.
  ///
  /// \param o output stream used for display
  /// \param e error to be displayed
  /// \return output stream
  std::ostream& operator<< (std::ostream& o, const SolverError& e);
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_ERROR_HH
