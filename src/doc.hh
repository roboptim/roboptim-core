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

   \mainpage User manual

   \section intro Introduction

   RobOptim Core defines a C++ common interface to several non linear problem
   solvers.

   The package is composed of a base class called
   optimization::GenericSolver and several sub-classes which transform
   the problem in order to pass it to an external solver.


   The interface relies heavily on Boost for:
   <ul>
   <li>advanced typing (Boost.Optional and Boost.Variant),</li>
   <li>and meta-programming
   (Boost.StaticAssert, Boost.TypeTraits, Boost.MPL).</li>
   </ul>

   Matrix manipulation is handled by default by the Eigen matrix.

   New users might want to check out the idea behind Boost.Optional
   and Boost.Variant are these types are present in the public
   interface.

   Users do not have to know about meta-programming to use this library.


   To get basic knowledge about the library, you might want to check out the
   \ref quickstart page.


   Table of content:
   - \subpage quickstart
   - \subpage filters
   - \subpage faq
   - \subpage design
   - \subpage bridge


   \section reporting Reporting bugs

   As this package is still in its early development steps, bugs report
   and enhancement proposals are highly welcomed.

   To report a bug, please go this package's
   <a href="http://www.roboptim.net/">webpage</a>.

   To discuss about this package, please use <a
   href="https://groups.google.com/forum/?fromgroups#!forum/roboptim"
   >roboptim@googlegroups.com</a>.
*/


/**
   \page quickstart Quick start
   \htmlonly
    <script type="text/x-mathjax-config">
      MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
    </script>
    <script type="text/javascript"
	    src="https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
    </script>
    <link href="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/8.0/styles/github.min.css" rel="stylesheet">
    <script type="text/javascript"
    src="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/8.0/highlight.min.js">
    </script>
    <script>hljs.initHighlightingOnLoad();</script>
   \endhtmlonly
   \htmlinclude getting-started.html
*/

/**
   \page filters Function filters
   \htmlonly
    <script type="text/x-mathjax-config">
      MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
    </script>
    <script type="text/javascript"
	    src="https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
    </script>
    <link href="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/8.0/styles/github.min.css" rel="stylesheet">
    <script type="text/javascript"
    src="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/8.0/highlight.min.js">
    </script>
    <script>hljs.initHighlightingOnLoad();</script>
   \endhtmlonly
   \htmlinclude filters.html
*/

/**
   \page faq Frequently Asked Questions (FAQ)

   \section avl_solvers What solvers are currently available?

   Supported solvers are available on seperated packages, please check
   <a href="http://www.roboptim.net/">the project webpage</a> of the
   supported solvers list.

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

   This package is divided into three parts:
   <ul>
   <li>Functions which represent different kinds of mathematical functions and
   their associated features (gradient, hessian, etc.).</li>
   <li>A problem class defining a whole optimization problem including some
   technical details (scales).</li>
   <li>A solver hierarchy defining solvers that are working on a class of
   optimizations problem (for instance, a QP solver solves problems with a
   quadratic objective function and linear constraints).</li>
   </ul>

   \section fct The function hierarchy

   The function hierarchy is an abstract hierarchy which defines
   <b>meta-functions</b> in the sense that the user has to derive from
   these classes to define its <b>real</b> function.

   \code
   struct MyLinearFunction : public LinearFunction {
     // FIXME: some code is missing
   };

   struct MyDerivableFunction : public DerivableFunction {
     // FIXME: some code is missing
   };
   // etc...
   \endcode

   Users can define as many functions as they want as long as they respect
   the constraints defined by the classes they inherit from:
   <ul>
   <li>Any function must be able to be evaluated (through operator()).</li>
   <li>A derivable function has a gradient/Jacobian matrix.</li>
   <li>A twice-derivable function (\f$C^2\f$) has a Hessian matrix.</li>
   <li>A quadratic function has the same constraints as a \f$C^2\f$
   function.</li>
   <li>A linear function has the same constraints except its Hessian
   matrix is null and computed automatically by the abstract class.</li>
   </ul>

   It is important to note that all of these functions are
   \f$\mathbb{R}^n \rightarrow \mathbb{R}^m\f$ functions.

   These functions have to be considered as m functions such that
   \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$.
   In particular, it means that in the gradient/Hessian methods, the
   second argument (integer) refers to which function you want to get
   the gradient/Hessian from.

   It avoids useless computations when the whole Jacobian is not needed.
   Hence, it also avoids the use of a tensor as this structure is particularly
   costly and not built-in in Eigen (the matrix library the package relies on).


   This set of meta-functions is completed by two generic implementations
   of quadratic and linear functions in respectively NumericQuadraticFunction
   and NumericLinearFunction.
   Those are real functions as the user is meant to instantiate them, not
   derive from them.


   \section pb The problem class

   The problem class defines a complete optimization problem.
   It is composed of:
   <ul>
   <li>A \f$\mathbb{R}^n \rightarrow \mathbb{R}\f$ cost
   (or objective) function.</li>
   <li>A set of functions constraints.</li>
   <li>Bounds on the input arguments.</li>
   <li>Bounds on the constraints functions output.</li>
   <li>Scales on the input arguments.</li>
   <li>Scales on the constraints functions output.</li>
   </ul>

   The problem class is templated by two parameters:
   <ul>
   <li>The objective's function type (F).</li>
   <li>The set of constraint's types (C).</li>
   </ul>

   Those two parameters define a class of problem.
   For instance, a QP solver solvers problems where the objective function
   is quadratic and the constraints are linear.

   Each class of problem matches a set of solvers designed to
   solve exactly those problems.

   The copy constructor of the Problem class allows to make
   the problem more generic by replacing F or C by a more
   general type.

   Examples:
   <ul>
   <li>It is valid to convert a linear problem into a nonlinear
   problem and make a QP solve it.</li>
   <li>But, the opposite is not valid.</li>
   </ul>

   I.e.: you can always go higher in the function hierarchy
   but you cannot go deeper.

   However, this transformation (as any dynamic typing) causes to
   <b>forget</b> specific information and might slow down the
   process. For example: solving a linear problem as a nonlinear one
   is slower than directly solving it with methods specific to linear
   problems.


   \section slv The solver hierarchy


   The solver hierarchy is divided into three parts:
   <ul>
   <li>The generic solver.</li>
   <li>The solver for a class of problems.</li>
   <li>The bridges (the leaves in the hierarchy's tree).</li>
   </ul>

   \subsection gs Generic solver

   The generic solver is an abstract class which is the root of the
   hierarchy. All the solvers have to inherit from this class directly or
   indirectly.

   It only defines three methods:
   <ul>
   <li><code>solve</code> (virtual pure)</li>
   <li><code>getMinimum</code></li>
   <li><code>reset</code></li>
   </ul>

   The method <code>solve</code> has to fill the protected attribute
   <code>result_</code>. It is implemented by the bridges at the bottom of the
   hierarchy.

   By default <code>result_</code> contains an instance of the
   <code>No_Solution</code> class.  It has to be changed to an instance of
   <code>Result</code>, <code>ResultWithWarnings</code> or
   <code>SolverError</code>.

   <ul>
   <li><code>No_Solution</code>: represents the fact that solve has never been
   called. A user should never retrieve an instance of <code>No_Solution</code>.
   </li>
   <li><code>Result</code>, <code>ResultWithWarnings</code>: represents a
   <strong>valid</strong> result.  If <code>ResultWithWarnings</code> is used,
   some non-critical problems might have happened and are specified in the
   object (minor numerical instabilities for instance).</li>
   <li><code>SolverError</code>: indicates that the optimization has
   <strong>failed</strong>.</li>
   </ul>

   The <code>solve</code> method should rarely be called by the user.
   Instead, the user calls <code>getMinimum</code> which calls
   <code>solve</code> if required.
   If <code>getMinimum</code> is called several times, the problem is only
   solved once.

   The <code>reset</code> method should be called when one wants to manually
   force the recomputation of the solution.


   \subsection solver Solver

   The <code>Solver</code> class is parametrized by two parameters as the
   <code>Problem</code> class.
   It defines an abstract solver for a class of optimization problems.

   For instance, a QP should inherit from
   <code>Solver<QuadraticFunction, LinearFunction></code>.

   This level of the hierarchy integrates the notion of <code>Problem</code>,
   hence the <code>problem</code> method is defined to access the problem. Once
   a solver is created with a problem, the problem can never be modified again.

   \subsection briges Bridges

   The leaves of the hierarchy are bridges. The role of a bridge is to convert
   the generic representation of the problem into the solver's representation.
   It might also emulate some missing features of the back-end solver.

   A bridge mainly implements the <code>solve</code> method. It may also
   expose the underlying solver's internal mechanism for fine tuning.
*/


/**
   \page bridge How to link the library with a new solver?


   This part assumes the reader went through the \ref design page and is
   familiar with the user interface.

   \section tutorial Intregrating a solver step by step.

   \subsection prereq Prerequisite

   The first step is determining if there is any interest in integrating
   the solver.

   The solver should target one of the following optimization problems:
   <ul>
   <li>Nonlinear optimization</li>
   <li>Linear optimization</li>
   </ul>
   ...with or without constraints.

   It should also support bounds and scales. If not, emulating them
   should be possible.

   Integrating solvers that solve very different problems will likely
   not match the interface and lead to major change in the package.
   These changes are beyond the scope of this document and should be
   discussed on the mailing-list.


   \subsection pkg Modifying the package.

   Basically, adding a new solver means adding a new leaf in the solver
   hierarchy.

   The new header and source files should be added in the <code>src</code>
   directory.

   The naming convention is CamlCase, the name should be the solver's name
   suffixed by <code>Solver</code>.

   This plug-in must be provided as a separate package to avoid
   cluttering RobOptim Core with useless dependencies.

   \subsection impl Implementing the bridge.

   The first step is choosing from which <code>Solver</code> you want to
   inherit from. The <code>F</code> parameter should be straightforward: it
   is the kind of objective function you expect.
   For the constraints, there are two cases:
   <ul>
   <li>If there is a unique constraint type: set <code>C</code> to
   <code>const MyConstraintType*</code> where <code>MyConstraintType</code>
   is the solver's constraints type.</li>
   <li>If you want to discriminate your constraints into different categories,
   you will have to use a Boost.Variant. <code>C</code> should be:
   <code>boost::variant<const MyConstraintType1*, const MyConstraintType2*>
   </code>
   </ul>

   Then, the <code>solve</code> method should be implemented in the bridge.


   Following guidelines should be respected by any bridge:
   <ul>
   <li>Be sure to call <code>reset</code> if the result has to be recomputed
   the next time <code>getMinimum</code> is called.</li>
   <li>Be sure that <code>solve</code> puts the computed value in the
   <code>result_</code> attribute.</li>
   <li>Solver's arguments should be exposed to users through the class.</li>
   <li>The solver should not display anything by default. If possible, use
   the <code>print</code> method to display internal states.</li>
   </ul>


   \subsection ccl Conclusion

   The dummy solver can be a good starting point for anyone who wants to add
   a new solver as the code can be considered as the minimum stub to get a
   compilable bridge. It should also be quite easy to understand.

   If you write a new bridge, please post an annoucement to the RobOptim
   Google group.
*/


/// \namespace roboptim
/// \brief Meta-functions, functions and solver-related classes.


/// \namespace roboptim::visualization
/// \brief Graphical visualization
///
/// Visualization-related code. Only Gnuplot and Matplotlib are currently
/// supported.


/// \namespace roboptim::visualization::gnuplot
/// \brief Gnuplot rendering
///
/// Gnuplot display classes.


/// \namespace roboptim::visualization::matplotlib
/// \brief Matplotlib rendering
///
/// Matplotlib display classes.


/// \defgroup roboptim_meta_function Mathematical abstract functions
/// \defgroup roboptim_function Mathematical functions
/// \defgroup roboptim_filter Function filters
/// \defgroup roboptim_problem Optimization problems
/// \defgroup roboptim_solver Optimization solvers
/// \defgroup roboptim_visualization Visualization
