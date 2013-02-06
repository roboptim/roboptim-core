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
#include <roboptim/core/identity-function.hh>
#include <roboptim/core/util.hh>

namespace roboptim
{
  IdentityFunction::IdentityFunction (const vector_t& offset)
    throw ()
    : LinearFunction (offset.size (), offset.size (), "identity function"),
      offset_ (offset)
  {
  }

  IdentityFunction::~IdentityFunction () throw ()
  {
  }

  void
  IdentityFunction::impl_compute (result_t result,
				  argument_t argument)
    const throw ()
  {
    //FIXME: why is it necessary?
    for (unsigned i = 0; i < result.size (); ++i)
      result[i] = argument[i] + this->offset_[i];
  }

  void
  IdentityFunction::impl_jacobian (jacobian_t jacobian,
				   argument_t) const throw ()
  {
    jacobian.diagonal () =
      Eigen::VectorXd::Constant (jacobian.diagonal ().size (), 1.);
    //jacobian_t::Identity (jacobian.rows (), jacobian.cols ());
  }

  void
  IdentityFunction::impl_gradient (gradient_t gradient,
				   argument_t,
				   size_type idFunction) const throw ()
  {
    gradient[idFunction] = 1.;
  }

  std::ostream&
  IdentityFunction::print (std::ostream& o) const throw ()
  {
    return o << "Identity function" << incindent << iendl
             << "offset = " << this->offset_ << iendl
             << decindent;
  }

} // end of namespace roboptim
