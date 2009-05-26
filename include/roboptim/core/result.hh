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
 * \brief Declaration of the Result class.
 */

#ifndef ROBOPTIM_CORE_RESULT_HH
# define ROBOPTIM_CORE_RESULT_HH
# include <iostream>
# include <vector>

# include <roboptim/core/function.hh>

namespace roboptim
{
  /// \brief Result class, used to communicate the result.
  class Result
  {
  public:
    typedef Function::size_type size_type;
    typedef Function::vector_t vector_t;

    explicit Result (const size_type,
                     const size_type = 1) throw ();
    virtual ~Result () throw ();

    virtual std::ostream& print (std::ostream& o) const throw ();

    size_type inputSize;
    size_type outputSize;
    vector_t x;
    vector_t value;
    vector_t lambda;
  };

  std::ostream& operator<< (std::ostream& o, const Result& r);
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_RESULT_HH
