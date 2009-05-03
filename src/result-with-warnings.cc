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
#include <roboptim/core/result-with-warnings.hh>
#include <roboptim/core/solver-warning.hh>
#include <roboptim/core/util.hh>

namespace roboptim
{
  ResultWithWarnings::ResultWithWarnings (const Function::size_type n,
                                          const Function::size_type m)
    throw ()
    : Result (n, m)
  {
  }

  std::ostream&
  ResultWithWarnings::print (std::ostream& o) const throw ()
  {
    Result::print (o);
    return o << incindent << iendl << "Warnings: " << warnings << decindent;
  }

  ResultWithWarnings::~ResultWithWarnings () throw ()
  {
  }

} // end of namespace roboptim
