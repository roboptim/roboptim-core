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
#include <roboptim/core/numeric-linear-function.hh>
#include <roboptim/core/util.hh>

namespace roboptim
{
  NumericLinearFunction::NumericLinearFunction (const matrix_t& a,
                                                const vector_t& b)
    throw ()
    : LinearFunction (a.cols (), a.rows (), "numeric linear function"),
      a_ (a),
      b_ (b)
  {
    assert (b.size () == outputSize ());
  }

  NumericLinearFunction::~NumericLinearFunction () throw ()
  {
  }


  // A * x + b
  void
  NumericLinearFunction::impl_compute (result_t& result,
				       const argument_t& argument)
    const throw ()
  {
    result.noalias () = a_* argument;
    result += b_;
  }

  // A
  void
  NumericLinearFunction::impl_jacobian (jacobian_t& jacobian,
					const argument_t&) const throw ()
  {
    jacobian = this->a_;
  }

  // A(i)
  void
  NumericLinearFunction::impl_gradient (gradient_t& gradient,
					const argument_t&,
					size_type idFunction) const throw ()
  {
    for (size_type j = 0; j < inputSize (); ++j)
      gradient[j] = a_ (idFunction, j);
  }

  std::ostream&
  NumericLinearFunction::print (std::ostream& o) const throw ()
  {
    return o << "Numeric linear function" << incindent << iendl
             << "A = " << this->a_ << iendl
             << "B = " << this->b_
             << decindent;
  }

} // end of namespace roboptim
