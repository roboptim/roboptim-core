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

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>


#include <roboptim/core/indent.hh>
#include <roboptim/core/numeric-quadratic-function.hh>

namespace roboptim
{
  NumericQuadraticFunction::NumericQuadraticFunction (const symmetric_t& a,
                                                      const vector_t& b)
    throw ()
    : QuadraticFunction (a.size1 (), 1),
      a_ (a),
      b_ (b)
  {
    assert (a.size1 () == a.size2 () && a.size2 () == b.size ());
  }


  NumericQuadraticFunction::~NumericQuadraticFunction () throw ()
  {
  }


  // 1/2 * x^T * A * x + b^T * x
  void
  NumericQuadraticFunction::impl_compute (result_t& result,
					  const argument_t& argument)
    const throw ()
  {
    using namespace boost::numeric::ublas;

    result (0) = inner_prod
      (prod (trans (argument), a_), argument) / 2
      + inner_prod (trans (b_), argument);
  }

  // x * A + b
  void
  NumericQuadraticFunction::impl_gradient (result_t& result,
					   const argument_t& x,
					   size_type) const throw ()
  {
    using namespace boost::numeric::ublas;
    noalias (result) = prod (x, a_) + b_;
  }

  // A
  void
  NumericQuadraticFunction::impl_hessian (hessian_t& hessian,
					  const argument_t&,
					  size_type) const throw ()
  {
    hessian = a_;
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
