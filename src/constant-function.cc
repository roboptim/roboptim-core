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

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include <roboptim/core/indent.hh>
#include <roboptim/core/constant-function.hh>

namespace roboptim
{
  ConstantFunction::ConstantFunction (const vector_t& offset)
    throw ()
    : LinearFunction (offset.size (), offset.size (), "constant function"),
      offset_ (offset)
  {
  }

  ConstantFunction::~ConstantFunction () throw ()
  {
  }

  void
  ConstantFunction::impl_compute (result_t& result,
				  const argument_t&)
    const throw ()
  {
    result = this->offset_;
  }

  void
  ConstantFunction::impl_jacobian (jacobian_t& jacobian,
				   const argument_t&) const throw ()
  {
    jacobian.clear ();
  }

  void
  ConstantFunction::impl_gradient (gradient_t& gradient,
				   const argument_t&,
				   size_type) const throw ()
  {
    gradient.clear ();
  }

  std::ostream&
  ConstantFunction::print (std::ostream& o) const throw ()
  {
    return o << "Constant function" << incindent << iendl
             << "offset = " << this->offset_ << iendl
             << decindent;
  }

} // end of namespace roboptim
