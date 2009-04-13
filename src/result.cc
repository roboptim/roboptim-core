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
 * \brief Implementation of the Result class.
 */

#include <iostream>
#include <vector>

#include <boost/numeric/ublas/io.hpp>

#include <liboptimization/function.hh>
#include <liboptimization/indent.hh>
#include <liboptimization/result.hh>
#include <liboptimization/util.hh>

namespace optimization
{
  Result::Result (const Function::size_type n_) throw ()
    : n (n_),
      x (n),
      value (),
      lambda (n)
  {
  }

  Result::Result (const Result& r) throw ()
    : n (r.n),
      x (r.x),
      value (r.value),
      lambda (r.lambda)
  {
  }

  Result*
  Result::operator= (const Result& r) throw ()
  {
    delete this;
    return new Result (r);
  }

  Result::~Result () throw ()
  {
  }

  std::ostream&
  Result::print (std::ostream& o) const throw ()
  {
    o << "Result: " << incindent << iendl
      << "Dimension: " << n << iendl
      << "X: " << x << iendl
      << "Value: " << value << iendl
      << "Lambda: " << lambda
      << decindent;
    return o;
  }

  std::ostream& operator<< (std::ostream& o, const Result& r)
  {
    return r.print (o);
  }

} // end of namespace optimization
