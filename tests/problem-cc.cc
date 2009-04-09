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

#include "common.hh"
#include <liboptimization/derivable-function.hh>
#include <liboptimization/problem.hh>

using namespace optimization;

struct F : public DerivableFunction
{
public:
  F () throw () : DerivableFunction (1)
  {}

  virtual value_type
  operator () (const vector_t&) const throw ()
  {
    return 0.;
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);
    grad.clear ();
    return grad;
  }
};

int run_test ()
{
  typedef Problem<DerivableFunction, const DerivableFunction*> problemSrc_t;
  typedef Problem<Function, const Function*> problemDst_t;

  F f;

  problemSrc_t pbSrc (f);
  problemDst_t pbDst (pbSrc);

  // Check that the problem has really been copied.
  assert (&pbSrc.function () == &pbDst.function ());
  assert (pbSrc.constraints ().size () == pbDst.constraints ().size ());

  for (unsigned i = 0; i < pbSrc.constraints ().size (); ++i)
    assert (pbSrc.constraints ()[i] == pbDst.constraints ()[i]);

  return 0;
}

GENERATE_TEST ()
