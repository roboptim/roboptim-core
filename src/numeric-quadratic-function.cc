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
 * \brief Implementation of the NumericQuadraticFunction class.
 */

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>


#include <roboptim/core/indent.hh>
#include <roboptim/core/numeric-quadratic-function.hh>

namespace roboptim
{
  NumericQuadraticFunction::NumericQuadraticFunction (const matrix_t& a,
                                                      const vector_t& b)
    throw ()
    : QuadraticFunction (a.size1 (), 1),
      a_ (a),
      b_ (b)
  {
    assert (a.size1 () == a.size2 () && a.size2 () == b.size ());
  }

  // 1/2 * x^T * A * x + b^T * x
  NumericQuadraticFunction::vector_t
  NumericQuadraticFunction::operator () (const vector_t& x) const throw ()
  {
    using namespace boost::numeric::ublas;

    vector_t res (1);
    res (0) = inner_prod
      (prod (trans (x), a_), x) / 2 + inner_prod (trans (b_), x);
    return res;
  }

  // x * A + b
  NumericQuadraticFunction::gradient_t
  NumericQuadraticFunction::gradient (const vector_t& x, int) const throw ()
  {
    using namespace boost::numeric::ublas;
    return prod (x, a_) + b_;
  }

  // A
  NumericQuadraticFunction::hessian_t
  NumericQuadraticFunction::hessian (const vector_t&, int) const throw ()
  {
    return a_;
  }

  std::ostream&
  NumericQuadraticFunction::print (std::ostream& o) const throw ()
  {
    return o << "Numeric quadratic function" << incindent << iendl
             << "A = " << a_ << iendl
             << "B = " << b_
             << decindent;
  }

} // end of namespace roboptim
