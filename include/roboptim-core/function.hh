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
 * \brief Declaration of the Function class.
 */

#ifndef OPTIMIZATION_FUNCTION_HH
# define OPTIMIZATION_FUNCTION_HH
# include <iostream>
# include <limits>
# include <utility>

# include <boost/numeric/ublas/matrix.hpp>
# include <boost/numeric/ublas/vector.hpp>

# include <roboptim-core/fwd.hh>

namespace roboptim
{
  namespace ublas = boost::numeric::ublas;

  /// \brief Define a mathematical function.
  class Function
  {
  public:
    /// Values type.
    typedef double value_type;
    /// Size type.
    typedef unsigned size_type;

    /// Vector type.
    typedef ublas::vector<value_type> vector_t;

    /// Matrix type.
    typedef ublas::matrix<value_type> matrix_t;

    /// Problem dimension.
    const size_type n;

    /// Result dimension.
    const size_type m;

    /// Get the value that symbolizes infinity.
    static const value_type infinity () throw ()
    {
      return std::numeric_limits<Function::value_type>::infinity ();
    }

    /// Constructor.
    /// \param n function arity
    /// \param m result size
    Function (size_type n, size_type m = 1) throw ();

    /// Destructor.
    virtual ~Function () throw ();

    /// Function.
    virtual vector_t operator () (const vector_t&) const throw () = 0;

    /// Display function type.
    virtual std::ostream& print (std::ostream&) const throw ();
  };

  std::ostream& operator<< (std::ostream&, const Function&);
} // end of namespace roboptim

#endif //! OPTIMIZATION_FUNCTION_HH
