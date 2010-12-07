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
#include <roboptim/core/identity-function.hh>

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
  IdentityFunction::impl_compute (result_t& result,
				  const argument_t& argument)
    const throw ()
  {
    using namespace boost::numeric::ublas;
    result = argument + this->offset_;
  }

  void
  IdentityFunction::impl_jacobian (jacobian_t& jacobian,
				   const argument_t&) const throw ()
  {
    using namespace boost::numeric::ublas;
    jacobian = identity_matrix<value_type> (jacobianSize ().first,
					    jacobianSize ().second);
  }

  void
  IdentityFunction::impl_gradient (gradient_t& gradient,
				   const argument_t& ,
				   size_type idFunction) const throw ()
  {
    using namespace boost::numeric::ublas;
    gradient.clear ();
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
