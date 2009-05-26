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
 * \brief Implementation of the Result class.
 */

#include <iostream>
#include <vector>

#include <boost/numeric/ublas/io.hpp>

#include <roboptim/core/function.hh>
#include <roboptim/core/indent.hh>
#include <roboptim/core/result.hh>
#include <roboptim/core/util.hh>

namespace roboptim
{
  Result::Result (const Function::size_type n_,
                  const Function::size_type m_) throw ()
    : n (n_),
      m (m_),
      x (n),
      value (m),
      lambda ()
  {
    x.clear ();
    value.clear ();
    lambda.clear ();
  }

  Result::Result (const Result& r) throw ()
    : n (r.n),
      m (r.m),
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

} // end of namespace roboptim
