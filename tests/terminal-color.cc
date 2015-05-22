// Copyright (C) 2015 by Benjamin Chrétien, CNRS-LIRMM.
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

#include <roboptim/core/terminal-color.hh>

using namespace roboptim;

BOOST_FIXTURE_TEST_SUITE (core, TestSuiteConfiguration)

std::ostream& testStream (std::ostream& o)
{
  o << fg::red << "red" << fg::reset << std::endl;
  o << fg::green << "green" << fg::reset << std::endl;
  o << fg::yellow << "yellow" << fg::reset << std::endl;
  o << fg::blue << "blue" << fg::reset << std::endl;
  o << fg::orange << "orange" << fg::reset << std::endl;

  o << fg::ok << "ok" << fg::reset << std::endl;
  o << fg::warn << "warn" << fg::reset << std::endl;
  o << fg::fail << "fail" << fg::reset << std::endl;

  return o;
}

BOOST_AUTO_TEST_CASE (terminal_color)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output_normal = retrievePattern ("terminal-color-normal");

  boost::shared_ptr<boost::test_tools::output_test_stream>
    output_tty;

  testStream (*output_normal);

  std::cout << output_normal->str () << std::endl;
  BOOST_CHECK (output_normal->match_pattern ());

  if (fg::isTtyStream (std::cout))
  {
    output_tty = retrievePattern ("terminal-color-tty");
  }
  else
  {
    std::cout << "Note: std::cout not registered as TTY" << std::endl;
    output_tty = retrievePattern ("terminal-color-normal");
  }

  // Temporarily redirect stdout to stringstream for testing
  std::streambuf* coutbuf = std::cout.rdbuf ();
  std::ostringstream coutstr;
  std::cout.rdbuf (coutstr.rdbuf());

  testStream (coutstr);

  // Reset std::cout to standard output
  std::cout.rdbuf (coutbuf);
  (*output_tty) << coutstr.str () << std::endl;

  std::cout << output_tty->str () << std::endl;
  BOOST_CHECK (output_tty->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
