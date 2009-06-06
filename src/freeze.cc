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

#include <roboptim/core/freeze.hh>

namespace roboptim
{
  Freeze::Freeze (const size_t size, const frozenArguments_t fa) throw ()
    : DerivableFunction (size, 1),
      frozenArguments_ (fa)
  {
    // Check that given index id are valid.
    typedef frozenArguments_t::const_iterator citer_t;
    for (citer_t it = frozenArguments_.begin (); it != frozenArguments_.end ();
	 ++it)
      assert (it->first < size);
  }

  Freeze::~Freeze () throw ()
  {
  }

  void
  Freeze::impl_compute (result_t& res, const argument_t& x) const throw ()
  {
    res (0) = 0.;
    typedef frozenArguments_t::const_iterator citer_t;
    for (citer_t it = frozenArguments_.begin (); it != frozenArguments_.end ();
	 ++it)
      res (0) += (x[it->first] - it->second) * (x[it->first] - it->second);
    res (0) -= 1;
  }

  void
  Freeze::impl_gradient (gradient_t& grad, const argument_t& x, size_type i)
    const throw ()
  {
    grad.clear ();
    typedef frozenArguments_t::const_iterator citer_t;
    for (citer_t it = frozenArguments_.begin (); it != frozenArguments_.end ();
	 ++it)
      grad (it->first) += 2 * (x[it->first] - it->second);
  }
} // end of namespace roboptim.
