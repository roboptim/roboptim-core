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

#include "debug.hh"

#include <roboptim/core/indent.hh>
#include <roboptim/core/numeric-quadratic-function.hh>
#include <roboptim/core/util.hh>

namespace roboptim
{
  NumericQuadraticFunction::NumericQuadraticFunction (const symmetric_t& a,
                                                      const vector_t& b)
    throw ()
    : QuadraticFunction (a.rows (), 1, "numeric quadratic function"),
      a_ (a),
      b_ (b)
  {
    assert (a.rows () == a.cols () && a.cols () == b.size ());
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
    result (0) = .5*argument.dot(a_*argument) + argument.dot(b_);
  }

  // x * A + b
  void
  NumericQuadraticFunction::impl_gradient (result_t& result,
					   const argument_t& x,
					   size_type) const throw ()
  {
    result.noalias () = a_ * x + b_;
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
