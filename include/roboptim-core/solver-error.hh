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

/**
 * \brief Declaration of the SolverError class.
 */

#ifndef OPTIMIZATION_SOLVER_ERROR_HH
# define OPTIMIZATION_SOLVER_ERROR_HH
# include <iostream>
# include <stdexcept>

namespace roboptim
{
  /// \brief Base error.
  /// All other errors inherits this class.
  class SolverError : public std::runtime_error
  {
  public:
    explicit SolverError (const std::string& arg) throw ();

    virtual std::ostream& print (std::ostream&) const throw ();
  };

  std::ostream& operator<< (std::ostream&, const SolverError&);
} // end of namespace roboptim

#endif //! OPTIMIZATION_SOLVER_ERROR_HH
