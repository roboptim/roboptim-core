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

#ifndef ROBOPTIM_CORE_SOLVER_WARNING_HH
# define ROBOPTIM_CORE_SOLVER_WARNING_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <roboptim/core/solver-error.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Exception used for non-critical errors during optimization.
  ///
  /// This class is mainly used to populate the warning vector of the
  /// ResultWithWarnings class.
  class ROBOPTIM_DLLAPI SolverWarning : public SolverError
  {
  public:
    /// \brief Instantiate the class with a message.
    /// \param arg message
    explicit SolverWarning (const std::string& arg);

    /// \brief Display the problem on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream&) const;
  };

  /// @}
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_SOLVER_ERROR_HH
