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
 * \file src/function.hh
 *
 * \brief Declaration of the Function class.
 */

#ifndef OPTIMIZATION_FUNCTION_HH
# define OPTIMIZATION_FUNCTION_HH
# include <limits>
# include <utility>

# include <boost/numeric/ublas/matrix.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/optional/optional.hpp>

# include <optimization-fwd.hh>

namespace optimization
{
  namespace ublas = boost::numeric::ublas;

  /// Define a mathematical function
  /// (and possibly a jacobian/hessian).
  struct Function
  {
    /// Values type.
    typedef double value_type;
    /// Size type.
    typedef unsigned size_type;

    /// Vector type.
    typedef ublas::vector<value_type> vector_t;

    /// Matrix type.
    typedef ublas::matrix<value_type> matrix_t;

    /// Gradient type.
    typedef boost::optional<vector_t> gradient_t;
    /// Hessian type.
    typedef boost::optional<matrix_t> hessian_t;

    /// Bound type (lower, upper).
    /// Use -infinity / +infinity to disable a bound.
    typedef std::pair<value_type, value_type> bound_t;
    /// Vector of bound.
    typedef std::vector<bound_t> bounds_t;

    /// Function arity.
    const size_type n;
    /// Value that symbolizes infinity.
    const value_type infinity;

    /// Linearity tag.
    enum Linearity
      {
        LINEAR,
        QUADRATIC,
        NON_LINEAR
      };

    /// Constructor.
    /// \param n function arity
    /// \param infinity value that encodes infinity
    Function (size_type n, value_type infinity =
              std::numeric_limits<value_type>::infinity ()) throw ();

    /// Destructor.
    virtual ~Function () throw ();

    /// Function.
    virtual value_type operator () (const vector_t&) const throw () = 0;
    /// Gradient.
    virtual gradient_t gradient (const vector_t&) const throw ();
    /// Hessian.
    virtual hessian_t hessian (const vector_t&) const throw ();

    /// Function bounds.
    bound_t bound;

    /// Arguments bounds.
    bounds_t argBounds;

    /// Function linearity.
    Linearity linearity;
  };
} // end of namespace optimization

#endif //! OPTIMIZATION_FUNCTION_HH
