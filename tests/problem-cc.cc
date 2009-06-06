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

#include "common.hh"
#include <roboptim/core/derivable-function.hh>
#include <roboptim/core/problem.hh>

using namespace roboptim;

struct F : public DerivableFunction
{
public:
  F () throw () : DerivableFunction (1, 1)
  {}

  void impl_compute (result_t& result, const argument_t&) const throw ()
  {
    result.clear ();
  }

  void impl_gradient (gradient_t& gradient,
		      const argument_t& argument, size_type) const throw ()
  {
    gradient.clear ();
  }
};

// Check that a problem has really been copied.
#define CHECK_COPY(A, B)                                                \
  assert (&(A).function () == &(B).function ());                        \
  assert ((A).constraints ().size () == (B).constraints ().size ());    \
                                                                        \
  for (unsigned i = 0; i < (A).constraints ().size (); ++i)             \
    assert ((A).constraints ()[i] == (B).constraints ()[i])

int run_test ()
{
  typedef Problem<DerivableFunction, const DerivableFunction*> problemSrc_t;
  typedef Problem<Function, const Function*> problemDst_t;

  F f;

  problemSrc_t pbSrc (f);

  // Check with same type.
  {
    problemSrc_t pbDst (pbSrc);
    CHECK_COPY(pbSrc, pbDst);
  }

  // Check with a more general type.
  {
    problemDst_t pbDst (pbSrc);
    CHECK_COPY(pbSrc, pbDst);
  }

  return 0;
}

GENERATE_TEST ()
