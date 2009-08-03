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

#ifndef ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
# define ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <vector>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/result.hh>
# include <roboptim/core/solver-warning.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Represents the solution of an optimization problem when
  /// errors occurred during the solving process.
  ///
  /// This class is returned by a solver if a result has been
  /// found, but some problems happened.
  ///
  /// A vector or warnings is provided in this result, otherwise the
  /// class behaves like Result.
  class ResultWithWarnings : public Result
  {
  public:
    /// \brief Instantiate the class from an input/output size.
    ///
    /// \param inputSize input size
    /// \param outputSize output size
    explicit ResultWithWarnings (const Function::size_type inputSize,
                                 const Function::size_type outputSize = 1)
      throw ();

    ~ResultWithWarnings () throw ();

    /// \brief Vector of warnings.
    /// Each element of this vector is a potential problem that occurred during
    /// the optimization.
    std::vector<SolverWarning> warnings;


    /// \brief Display the result on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const throw ();
  };

  /// @}

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
