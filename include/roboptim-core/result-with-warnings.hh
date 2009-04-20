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
 * \brief Declaration of the ResultWithWarnings class.
 */

#ifndef ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
# define ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
# include <vector>

# include <roboptim-core/fwd.hh>
# include <roboptim-core/result.hh>
# include <roboptim-core/solver-warning.hh>

namespace roboptim
{
  /// Retrieved when a result has been found but some problems
  /// happened during the computation.
  class ResultWithWarnings : public Result
  {
  public:
    explicit ResultWithWarnings (const Function::size_type,
                                 const Function::size_type = 1)
      throw ();
    ~ResultWithWarnings () throw ();

    std::vector<SolverWarning> warnings;

    virtual std::ostream& print (std::ostream& o) const throw ();
  };

} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_WITH_WARNINGS_HH
