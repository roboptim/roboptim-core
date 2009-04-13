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
 * \brief Implementation of the NumericLinearFunction class.
 */

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include <liboptimization/indent.hh>
#include <liboptimization/numeric-linear-function.hh>

namespace optimization
{
  NumericLinearFunction::NumericLinearFunction (const vector_t& a,
                                                value_type b)
    throw ()
    : LinearFunction (a.size (), 1),
      a_ (a),
      b_ (b)
  {
  }

  // A * x + b
  NumericLinearFunction::vector_t
  NumericLinearFunction::operator () (const vector_t& x) const throw ()
  {
    using namespace boost::numeric::ublas;
    vector_t res (1);
    res(0) = inner_prod (a_, x) + b_;
    return res;
  }

  // A
  NumericLinearFunction::gradient_t
  NumericLinearFunction::gradient (const vector_t&) const throw ()
  {
    return a_;
  }

  std::ostream&
  NumericLinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Numeric linear function" << incindent << iendl
             << "A = " << a_ << iendl
             << "B = " << b_
             << decindent;
  }

} // end of namespace optimization
