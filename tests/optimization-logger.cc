// Copyright (C) 2014 by Benjamin Chretien, CNRS-LIRMM.
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

#include "shared-tests/fixture.hh"

#include <iostream>

#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <roboptim/core/io.hh>
#include <roboptim/core/solver-factory.hh>
#include <roboptim/core/optimization-logger.hh>
#include <roboptim/core/differentiable-function.hh>

using namespace roboptim;

// Define a simple function (non-differentiable)
struct F1 : public Function
{
  typedef Function parent_t;

  F1 () : parent_t (4, 1, "a * d * (a + b + c) + d\nDummy constraint")
  {}

  void impl_compute (result_ref result, const_argument_ref x)
    const
  {
    result (0) = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[3];
  }

  // No gradient, hessian.
};

// Define a simple function (sparse differentiable)
struct F2 : public DifferentiableSparseFunction
{
  typedef DifferentiableSparseFunction parent_t;

  F2 () : parent_t (4, 1, "a + b + c + d\nDummy constraint")
  {}

  void impl_compute (result_ref result, const_argument_ref x)
    const
  {
    result (0) = x[0] + x[1] + x[2] + x[3];
  }

  void impl_gradient (gradient_ref grad, const_argument_ref, size_type)
    const
  {
    grad.insert (0) = 1;
    grad.insert (1) = 1;
    grad.insert (2) = 1;
    grad.insert (3) = 1;
  }
};

bool findRegex (const std::string& text, const std::string& r)
{
  boost::xpressive::sregex rex = boost::xpressive::sregex::compile (r);
  return boost::xpressive::regex_search (text, rex);
}

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

// See: http://stackoverflow.com/a/596348/1043187
template <typename>
struct toString
{
  static char const* value () { return "unknown"; }
};

#define DEF_FUNC_TYPE(X)			\
  template <>					\
  struct toString<X>				\
  {						\
    static char const* value () { return #X; }	\
  }

DEF_FUNC_TYPE(F1);
DEF_FUNC_TYPE(F2);

template <>
struct toString<Function>
{
  static char const* value () { return ""; }
};

template <>
struct toString<DifferentiableSparseFunction>
{
  static char const* value () { return "-d-sparse"; }
};

template <typename T>
bool testXOR (const T& a, const T& b)
{
  return !a != !b;
}

template <typename F>
void testLogger
(boost::shared_ptr<boost::test_tools::output_test_stream> output,
 unsigned int requests)
{
  // Specify the solver that will be used.
  typedef Solver<typename F::traits_t> solver_t;

  // Instantiate the function and the problem.
  boost::shared_ptr<F> f = boost::make_shared<F> ();
  typename solver_t::problem_t pb (f);

  // Add a dummy constraint to test logging
  boost::shared_ptr<F> g = boost::make_shared<F> ();
  pb.addConstraint (g, F::makeInterval (-1., 1.));

  // Give a dummy starting point
  typename F::argument_t x (f->inputSize ());
  x.setZero ();
  pb.startingPoint () = x;

  // Instantiate the factory using the dummy solver.
  SolverFactory<solver_t>
    factory (std::string ("dummy")
             + toString<typename F::parent_t>::value ()
             + "-laststate",
             pb);

  // Retrieve a solver instance from the factory.
  solver_t& solver = factory ();

  // Add optimization logger.
  // Note: scoped to trigger the destructor.
  typedef OptimizationLogger<solver_t> logger_t;
  std::stringstream buffer;
  boost::filesystem::path log_path;

  {
    logger_t logger (solver,
        std::string ("/tmp/roboptim-core-tests/optimization-logger-")
        + toString<F>::value (), true, requests);

    // Get the Boost.Variant minimum.
    typename solver_t::result_t res = solver.minimum ();

    // Get the "real" minimum.
    solver.template getMinimum<SolverError> ();

    // Display problem and solver.
    (*output) << pb << std::endl
      << "---" << std::endl
      << solver << std::endl;

    logger.append ("Append test 1");
    logger << "Append test 2"
      << solver.problem () << "\n";

    log_path = logger.logPath ();

    // Test whether the logging directory exists
    BOOST_CHECK (boost::filesystem::exists (log_path));
    // Test whether journal.log exists
    BOOST_CHECK (boost::filesystem::exists (log_path / "journal.log"));
    // Test whether constraint-violation.csv exists
    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_CONSTRAINT_VIOLATION),
                            boost::filesystem::exists
                              (log_path / "constraint-violation-evolution.csv")));

    // Test whether appended strings are found
    std::ifstream journal ((log_path / "journal.log").string ().c_str ());
    // Load unfinished journal
    buffer << journal.rdbuf();

    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_X),
                            findRegex (buffer.str (), "- x:\n")));
    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_COST),
                            findRegex (buffer.str (), "- f\\(x\\):\n")));
    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_CONSTRAINT_VIOLATION),
                            findRegex (buffer.str (), "- viol_g\\(x\\):\n")));
    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_TIME),
                            findRegex (buffer.str (),
                                       "Elapsed time since last call:")));
    BOOST_CHECK (! testXOR (logger.isRequested (logger_t::LOG_SOLVER),
                            findRegex (buffer.str (), "Solver:\n")));

    buffer.clear ();
  }

  // Load finalized journal
  std::ifstream final_journal ((log_path / "journal.log").string ().c_str ());
  buffer << final_journal.rdbuf();

  // Note: triggered during the destructor of the logger
  BOOST_CHECK (! testXOR ((logger_t::LOG_TIME & requests) == logger_t::LOG_TIME,
                          findRegex (buffer.str (), " - total elapsed time:")));

  BOOST_CHECK (findRegex (buffer.str (), "Append test 1"));
  BOOST_CHECK (findRegex (buffer.str (), "Append test 2"));
  BOOST_CHECK (findRegex (buffer.str (), "Problem:\n"));
}

BOOST_AUTO_TEST_CASE (plugin)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("optimization-logger");

  typedef OptimizationLogger<Solver<F1::traits_t> > logger_t;
  testLogger<F1> (output, logger_t::FullLogging ());
  (*output) << "---" << std::endl;
  testLogger<F2> (output, logger_t::FullLogging ());
  (*output) << "---" << std::endl;
  testLogger<F1> (output, 0);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
