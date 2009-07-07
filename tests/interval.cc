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

#include <iostream>

#include <boost/tuple/tuple_io.hpp>

#include "common.hh"
#include <roboptim/core/function.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

struct MyFunctor
{
  void operator () (double t)
  {
    std::cout << t << std::endl;
  }
};

int run_test ()
{
  typedef Function::interval_t interval_t;
  typedef Function::discreteInterval_t discreteInterval_t;

  {
    interval_t i1 = Function::makeInterval (0., 5.);
    assert (Function::getLowerBound (i1) == 0.);
    assert (Function::getUpperBound (i1) == 5.);

    interval_t i2 = Function::makeInfiniteInterval ();
    double inf = Function::infinity ();
    // Just make sure inf != NaN.
    assert (inf == inf);
    assert (Function::getLowerBound (i2) == -inf);
    assert (Function::getUpperBound (i2) == inf);

    interval_t i3 = Function::makeLowerInterval (42.);
    assert (Function::getLowerBound (i3) == 42.);
    assert (Function::getUpperBound (i3) == inf);

    interval_t i4 = Function::makeUpperInterval (42.);
    assert (Function::getLowerBound (i4) == -inf);
    assert (Function::getUpperBound (i4) == 42.);

    std::cout << i1 << std::endl
	      << i2 << std::endl
	      << i3 << std::endl
	      << i4 << std::endl;
  }
  {
    discreteInterval_t i1 = Function::makeDiscreteInterval (0., 5., 1.);
    assert (Function::getLowerBound (i1) == 0.);
    assert (Function::getUpperBound (i1) == 5.);
    assert (Function::getStep (i1) == 1.);

    double inf = Function::infinity ();
    discreteInterval_t i2 = Function::makeDiscreteInterval (-inf, inf, .1);
    assert (Function::getLowerBound (i2) == -inf);
    assert (Function::getUpperBound (i2) == inf);
    assert (Function::getStep (i2) == .1);

    discreteInterval_t i3 = Function::makeDiscreteInterval
      (Function::makeInterval (1., 4.), .1);

    std::cout << i1 << std::endl
	      << i2 << std::endl
	      << i3 << std::endl;

    Function::foreach (i1, MyFunctor ());
  }

  return 0;
}

GENERATE_TEST ()
