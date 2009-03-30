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
   // Used solver.
   typedef IpoptSolver solver_t;

   // Objective function.
   double my_fun (const solver_t::array_t& x) { FIXME }

   // A constraint.
   double g_0 (const solver_t::array_t& x) { FIXME }

   // A gradient function.
   solver_t::array_t my_gradient (const solver_t::array_t& x) { FIXME }

   // A hessian function.
   solver_t::matrix_t my_hessian (const solver_t::array_t& x,
                                  const solver_t::constraints_t& g,
                                  double sigma_f,
                                  const solver_t::array_t& lambda) { FIXME }

   // A jacobian function.
   solver_t::matrix_t my_jacobian (const solver_t::array_t& x,
                                   const solver_t::constraints_t& g) { FIXME }

  // Declare a problem with 2 variables
  static const int nvar = 2;

  // Initialize solver
  solver_t solver (my_fun, nvar, my_gradient, my_hessian, my_jacobian);

  // Set the starting point.
  solver_t::array_t start (nvar);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  solver.setStartingPoint (start);

  // Set bound for all variables.
  // 1. < x_i < 5. (x_i in [1.;5.])
  for (int i = 0; i < nvar; ++i)
    solver.setBound (i, std::make_pair(1., 5.));

  // Add a constraint.
  // The constraint is that g_0(x) should be at least 10 and at most 50
  solver.getConstraints ().push_back (IpoptSolver::Constraint (g_0, 10., 25.));

  // Compute the minimum and retrieve the result.
  // Then, check if a minimum has been found.
  if (solver.getMinimum ().which () != IpoptSolver::SOLVER_VALUE)
    {
      // Failing...
      return 1;
    }

  // Get the result.
  // As long the problem is not changed, the result will not be recomputed.
  solver_t::array_t& result =
     boost::get<solver_t::array_t> (solver.getMinimum ());

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << my_fun (result) << std::endl;
  return 0;
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
