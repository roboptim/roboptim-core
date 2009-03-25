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
 * \file dummy.cc
 *
 * \brief Implementation of the dummy module (always fail).
 */

#include <solver.hh>

namespace optimization
{

  class DummySolver : public Solver
  {
  public:
    DummySolver (const function_t& fct) throw ()
      : Solver (fct)
    {
    }

    virtual ~DummySolver () throw ()
    {
    }

    virtual result_t
    getMinimum () throw ()
    {
      return result_t (SolverError ());
    }
  };

} // end of namespace optimization


extern "C"
{
  optimization::Solver*
  create (const optimization::DummySolver::function_t& arg)
  {
    return new optimization::DummySolver (arg);
  }

  void
  destroy (optimization::Solver* p)
  {
    delete p;
  }

  unsigned
  version ()
  {
    return optimization::Solver::PLUGIN_INTERFACE_VERSION;
  }
}
