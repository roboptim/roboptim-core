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

#ifndef ROBOPTIM_CORE_RESULT_HH
# define ROBOPTIM_CORE_RESULT_HH
# include <roboptim/core/sys.hh>
# include <roboptim/core/debug.hh>

# include <iostream>
# include <vector>

# include <roboptim/core/function.hh>

namespace roboptim
{
  /// \addtogroup roboptim_problem
  /// @{

  /// \brief Represents the solution of an optimization problem.
  ///
  /// This class is returned by a solver if a result has been
  /// found.
  /// It is a set of mutable fields representing the solution
  /// and its associated meta-information.
  class ROBOPTIM_DLLAPI Result
  {
  public:
    /// \brief Import size type from Function class.
    typedef Function::size_type size_type;
    /// \brief Import vector type from Function class.
    typedef Function::vector_t vector_t;

    /// \brief Instantiate a result and fix input/output sizes.
    ///
    /// \param inputSize input size value
    /// \param outputSize output size value
    explicit Result (const size_type inputSize,
		     const size_type outputSize = 1);

    virtual ~Result ();

    /// \brief Display the result on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const;

    /// \brief Input size (i.e. argument size).
    size_type inputSize;
    /// \brief Output size (i.e. result size).
    size_type outputSize;
    /// \brief Point found by the solver.
    vector_t x;
    /// \brief Function value at the solver found point.
    vector_t value;
    /// \brief Constraints final values.
    vector_t constraints;
    /// \brief Lagrange multipliers.
    vector_t lambda;
  };

  /// Example shows Result class use.
  /// \example result.cc

  /// @}

  /// \brief Override operator<< to handle result display.
  ///
  /// \param o output stream used for display
  /// \param r result to be displayed
  /// \return output stream
  ROBOPTIM_DLLAPI std::ostream& operator<< (std::ostream& o, const Result& r);
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_HH
