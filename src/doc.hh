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
 * \file src/doc.hh
 *
 * \brief Main documentation header.
 */

/**

   \mainpage

   \section intro Introduction

   liboptimization defines a C++ common interface to several non linear problem
   solvers.

   The package is composed of a base class called optimization::Solver and
   several sub-classes which transform the problem in order to pass it to an
   external solver.


   The interface relies heavily on Boost for:
   <ul>
   <li>function definition (Boost.Function),</li>
   <li>advanced typing (Boost.Optional and Boost.Variant),</li>
   <li>matrix manipulation (Boost.uBLAS).</li>
   </ul>


   \section tutorial Tutorial

   Here is a complete example of using Ipopt to solve a problem.

   \code
struct F : public Function
{
  F () : Function (4)
  {
    // Set bound for all variables.
    // 1. < x_i < 5. (x_i in [1.;5.])
    for (int i = 0; i - n > 0; ++i)
      argBounds[i] = std::make_pair(1., 5.);
  }

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
    grad[1] = x[0] * x[3];
    grad[2] = x[0] * x[3] + 1;
    grad[3] = x[0] * (x[0] + x[1] + x[2]);
    return grad;
  }

  virtual hessian_t
  hessian (const vector_t& x) const throw ()
  {
    matrix_t h (n, n);
    h (0, 0) = 2 * x[3];
    h (0, 1) = x[3];
    h (0, 2) = x[3];
    h (0, 3) = 2 * x[0] + x[1] + x[2];

    h (1, 0) = x[3];
    h (1, 1) = 0.;
    h (1, 2) = 0.;
    h (1, 3) = x[0];

    h (2, 0) = x[3];
    h (2, 1) = 0.;
    h (2, 2) = 0.;
    h (2, 3) = x[1];

    h (3, 0) = 2 * x[0] + x[1] + x[2];
    h (3, 1) = x[0];
    h (3, 2) = x[0];
    h (3, 3) = 0.;
    return h;
  }
};

struct G0 : public Function
{
  G0 ()
    : Function (4)
  {}

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0] * x[1] * x[2] * x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = x[1] * x[2] * x[3];
    grad[1] = x[0] * x[2] * x[3];
    grad[2] = x[0] * x[1] * x[3];
    grad[3] = x[0] * x[1] * x[2];
    return grad;
  }
};

struct G1 : public Function
{
  G1 ()
    : Function (4)
  {}

  virtual value_type
  operator () (const vector_t& x) const throw ()
  {
    return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
  }

  virtual gradient_t
  gradient (const vector_t& x) const throw ()
  {
    vector_t grad (n);

    grad[0] = 2 * x[0];
    grad[1] = 2 * x[1];
    grad[2] = 2 * x[2];
    grad[3] = 2 * x[3];
    return grad;
  }
};

int main ()
{
  Problem pb = Problem (F ());

  // Set the starting point.
  Function::vector_t start (pb.function.n);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  pb.start = start;

  pb.constraints.push_back (Problem::functionPtr_t (new G0 ()));
  pb.constraints.push_back (Problem::functionPtr_t (new G1 ()));

  // Initialize solver
  IpoptSolver solver (pb);

  // Compute the minimum and retrieve the result.
  IpoptSolver::result_t res = solver.getMinimum ();

  // Check if the minimization has succeed.
  if (res.which () != IpoptSolver::SOLVER_VALUE)
    {
      // Solver has failed.
      return 1;
    }

  // Get the result.
  IpoptSolver::vector_t& result = boost::get<IpoptSolver::vector_t> (res);

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << pb.function (result) << std::endl;
  return 0;
}
   \endcode

   \section faq Frequently Asked Questions (FAQ)

   \subsection avl_solvers What solvers are currently available?

   Supported solvers are:
   <ul>
   <li><a href="https://projects.coin-or.org/Ipopt/">Ipopt</a></li>
   </ul>

   Solvers we plan to support in the future:
   <ul>
   <li><a href="http://www.aemdesign.com/">CFSQP</a></li>
   <li><a href="http://vxl.sourceforge.net/">VXL/VNL</a></li>
   </ul>

   \subsection boost_optvar What are those Optional and Variant types?

   An optional type is a templated class which modelize the possibility
   in C++ to have "no value" in a variable.
   It is safer than passing a null pointer as an exception will be
   launched if you try to dereference a variable which has no content.

   See the following example:
   \code
   // Declare an empty optional int.
   boost::optional<int> optInt;

   // There is nothing in our optional int.
   // Check if this is really the case.
   if (optInt)
      assert (false);

   // Set a value in our optional int.
   optInt = 42;

   // Access our optional int.
   std::cout << *optInt << std::endl;
   \endcode


   A variant is an object-oriented way to define a C enum.
   Here is a small example:

   \code
   // x can be a string or an int.
   boost::variant<int, std::string> x;

   // Set x to 42.
   x = 42;

   // Which allows to determine what is the current type of x
   // First type (int) is zero, second (string) is one, etc.
   assert (x.which () == 0);

   // Set x to "hello world".
   x = "hello world";

   // Test again.
   assert (x.which () == 1);
   \endcode

   Please note that a variant can never be empty.


   See the <a href="http://www.boost.org/">Boost.Optional and Boost.Variant
   documentation</a> for more information.
*/

/*!
  \namespace optimization

  \brief Main namespace

  The namespace gathers all the classes of this library.
*/

/*!
  \namespace optimization::detail

  \brief Implementation namespace

  The namespace gathers implementation details which are of no-concern
  for the end user of this library.
*/
