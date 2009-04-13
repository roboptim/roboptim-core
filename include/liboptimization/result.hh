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
 * \brief Declaration of the Result class.
 */

#ifndef OPTIMIZATION_RESULT_HH
# define OPTIMIZATION_RESULT_HH
# include <iostream>
# include <vector>

# include <liboptimization/function.hh>

namespace optimization
{
  /// \brief Result class, used to communicate the result.
  class Result
  {
  public:
    explicit Result (const Function::size_type,
                     const Function::size_type = 1) throw ();
    explicit Result (const Result&) throw ();
    Result* operator= (const Result& r) throw ();
    virtual ~Result () throw ();

    virtual std::ostream& print (std::ostream& o) const throw ();

    const Function::size_type n;
    const Function::size_type m;
    Function::vector_t x;
    Function::vector_t value;
    Function::vector_t lambda;
  };

  std::ostream& operator<< (std::ostream& o, const Result& r);
} // end of namespace optimization

#endif //! OPTIMIZATION_RESULT_HH
