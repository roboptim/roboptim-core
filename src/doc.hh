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

   \mainpage liboptimization user manual

   \section intro Introduction

   liboptimization defines a C++ common interface to several non linear problem
   solvers.

   The package is composed of a base class called optimization::GenericSolver and
   several sub-classes which transform the problem in order to pass it to an
   external solver.


   The interface relies heavily on Boost for:
   <ul>
   <li>advanced typing (Boost.Optional and Boost.Variant),</li>
   <li>matrix manipulation (Boost.uBLAS).</li>
   </ul>

   To get basic knowledge about the library, you might want to check out the
   \ref quickstart page.


   Table of content:
   - \subpage quickstart
   - \subpage faq
   - \subpage design
   - \subpage bridge
*/


/**
   \page quickstart Quick start

   Solving a problem is done through several steps:

   - Define your cost function by deriving one kind of function, depending
   on whether or not you want to provide a jacobian and/or a hessian.
   - Define your constraints functions in the same manner.
   - Build an instance of problem matching your requirements.
   - Use one of the solvers to solve your problem.


   The following example defines a cost function F and two constraints G0 and G1.

   \section problem Problem definition

   The problem that will be solved is:

   \f$min_{x \in \mathbb{R}^4} x_1 x_4 (x_1 + x_2 + x_3) + x_3\f$

   \ldots with the following constraints:

   - \f$x_1 x_2 x_3 x_4 \geq 25\f$
   - \f$x_1^2 + x_2^2 + x_3^2 + x_4^2 = 40\f$
   - \f$1 \leq x_1, x_2, x_3, x_4 \leq 5\f$

   \section cost Defining the cost function.

   The library contains the following hierarchy of functions:
   - optimization::Function
   - optimization::DerivableFunction
   - optimization::TwiceDerivableFunction
   - optimization::QuadraticFunction
   - optimization::LinearFunction

   When defining a new function, you have to derive your new function from one
   of those classes. Depending on the class you derive from, you will be have
   to implement one or several methods:

   - operator () that returns the function's result has to be defined for all
   functions.
   - getGradient () which returns the function's gradient is to be
   defined for DerivableFunction and its subclasses.
   - getHessian () for TwiceDerivableFunction functions and its subclasses.

   It is usually recommended to derive from the deepest possible class of the
   hierarchy (deriving from TwiceDerivableFunction is better than
   DerivableFunction).

   Keep in mind that the type of the function represents the amount of
   information the solver will get, not the real nature of a function (it is
   possible to avoid defining a hessian by deriving from DerivableFunction,
   even if you function can be derived twice).

   In the following sample, a TwiceDerivableFunction will be defined.

   \code
class F : public TwiceDerivableFunction
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
   \endcode

   \section constraints Defining the constraints.

   A constraints is no different from a cost function and
   can be defined in the same way than a cost function.

   The following sample defines two constraints which are
   twice derivable functions.


   \code
class G0 : public TwiceDerivableFunction
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

class G1 : public TwiceDerivableFunction
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
   \endcode

   \section problem Building the problem and solving it.

   The last part of this tutorial covers how to build a problem and
   solve it. The steps are:

   - Instanciate your functions (cost functions and constraints).
   - Pass them to the problem.
   - Optional: set a starting point.
   - Instanciate a solver which solves your class of problem.
   - Solve the problem by calling getMinimum.

   \code
int main ()
{
  F f;
  G0 g0;
  G1 g1;

  Problem<TwiceDerivableFunction, TwiceDerivableFunction> pb (f);
  pb.getConstraints ().push_back (&g0);
  pb.getConstraints ().push_back (&g1);

  // Set the starting point (optional).
  Function::vector_t start (f.n);
  start[0] = 1., start[1] = 5., start[2] = 5., start[3] = 1.;
  pb.getStartingPoint () = start;

  // Initialize solver
  IpoptSolver solver (pb);

  // Compute the minimum and retrieve the result.
  IpoptSolver::result_t res = solver.getMinimum ();

  // Check if the minimization has succeed.
  if (res.which () != IpoptSolver::SOLVER_VALUE)
    {
      std::cout << "A solution should have been found. Failing..."
                << std::endl;
      return 1;
    }

  // Get the result.
  IpoptSolver::vector_t& result = boost::get<IpoptSolver::vector_t> (res);

  // Display the result.
  std::cout << "A solution has been found: " << std::endl;
  std::cout << result << std::endl;
  std::cout << "f(*x) = " << solver.getProblem ().getFunction () (result) << std::endl;
  return 0;
}
   \endcode
*/

/**
   \page faq Frequently Asked Questions (FAQ)

   \section avl_solvers What solvers are currently available?

   Supported solvers are:
   <ul>
   <li><a href="https://projects.coin-or.org/Ipopt/">Ipopt</a></li>
   <li><a href="http://www.aemdesign.com/">CFSQP</a></li>
   </ul>

   Solvers we plan to support in the future:
   <ul>
   <li><a href="http://vxl.sourceforge.net/">Optimization algorithms from VXL/VNL</a></li>
   </ul>

   \section boost_optvar What are those Optional and Variant types?

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


/**
   \page design Design choices and libraries' internals

   TO BE DONE

   \section fct The function hierarchy
   \section pb The problem class
   \section slv The solver hierarchy
*/


/**
   \page bridge How to link the library with a new solver?

   TO BE DONE

   \section slv How to extend the solver hierarchy?
   \section tutorial Intregrating a solver step by step.
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
