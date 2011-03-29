// Copyright (C) 2010 by Thomas Moulard, AIST, CNRS, INRIA.
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

#undef NDEBUG

#include "shared-tests/common.hh"

#include <iostream>

#include <roboptim/core/io.hh>
#include <roboptim/core/derivable-function.hh>
#include <roboptim/core/util.hh>
#include <roboptim/core/filter/split.hh>

using namespace roboptim;

struct F : public DerivableFunction
{
  F () : DerivableFunction (1, 10, "f_n (x) = n * x")
  {}

  void impl_compute (result_t& res, const argument_t& argument) const throw ()
  {
    res.clear ();
    for (size_type i = 0; i < outputSize (); ++i)
      res[i] = i * argument[0];
  }

  void impl_gradient (gradient_t& grad, const argument_t&,
		      size_type functionId) const throw ()
  {
    grad.clear ();
    grad[0] = functionId;
  }
};

int run_test ()
{
  boost::shared_ptr<F> f (new F ());

  for (unsigned id = 0; id < 10; ++id)
    {
      Split<DerivableFunction> splitF (f, id);

      std::cout << splitF << ":" << std::endl
		<< std::endl;
      
      Function::vector_t x (1);
      for (double i = 0.; i < 10.; i += 0.5)
	{
	  x[0] = i;
	  std::cout << splitF (x) << std::endl;
	  std::cout << splitF (x) << std::endl;
	  //assert ((*f) (x)[0] == splitF (x)[id]);
	  
	  std::cout << splitF.gradient (x) << std::endl;
	  std::cout << splitF.gradient (x) << std::endl;
	}
    }
  return 0;
}

GENERATE_TEST ()
