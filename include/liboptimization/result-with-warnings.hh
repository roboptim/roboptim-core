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
 * \brief Declaration of the ResultWithWarnings class.
 */

#ifndef OPTIMIZATION_RESULT_WITH_WARNINGS_HH
# define OPTIMIZATION_RESULT_WITH_WARNINGS_HH
# include <vector>

# include <liboptimization/fwd.hh>
# include <liboptimization/result.hh>
# include <liboptimization/solver-warning.hh>

namespace optimization
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

} // end of namespace optimization

#endif //! OPTIMIZATION_RESULT_WITH_WARNINGS_HH
