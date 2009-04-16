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
 * \brief Implementation of the NumericLinearFunction class.
 */

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include <roboptim-core/indent.hh>
#include <roboptim-core/numeric-linear-function.hh>

namespace roboptim
{
  NumericLinearFunction::NumericLinearFunction (const matrix_t& a,
                                                const vector_t& b)
    throw ()
    : LinearFunction (a.size2 (), a.size1 ()),
      a_ (a),
      b_ (b)
  {
    assert (b.size () == m);
  }

  // A * x + b
  NumericLinearFunction::vector_t
  NumericLinearFunction::operator () (const vector_t& x) const throw ()
  {
    using namespace boost::numeric::ublas;
    vector_t res (m);
    return prod (a_, x) + b_;
  }

  // A
  NumericLinearFunction::jacobian_t
  NumericLinearFunction::jacobian (const vector_t&) const throw ()
  {
    return a_;
  }

  // A(i)
  NumericLinearFunction::gradient_t
  NumericLinearFunction::gradient (const vector_t&, int i) const throw ()
  {
    gradient_t grad (n);
    for (unsigned j = 0; j < n; ++i)
      grad[j] = a_ (i, j);
    return grad;
  }

  std::ostream&
  NumericLinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Numeric linear function" << incindent << iendl
             << "A = " << a_ << iendl
             << "B = " << b_
             << decindent;
  }

} // end of namespace roboptim
