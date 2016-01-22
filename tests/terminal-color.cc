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
  using namespace std;
  using namespace roboptim::fg;

  o << "normal text" << endl;

  o << red << "red" << resetfg << " text" << endl;
  o << green << "green" << resetfg << " text" << endl;
  o << yellow << "yellow" << resetfg << " text" << endl;
  o << blue << "blue" << resetfg << " text" << endl;
  o << orange << "orange" << resetfg << " text" << endl;

  o << bold << "bold" << normal << " text" << endl;
  o << red << bold << "bold red" << reset << " text" << endl;
  o << green << bold << "bold green" << reset << " text" << endl;
  o << yellow << bold << "bold yellow" << reset << " text" << endl;
  o << blue << bold << "bold blue" << reset << " text" << endl;
  o << orange << bold << "bold orange" << reset << " text" << endl;

  o << italic << "italic" << normal << " text" << endl;
  o << red << italic << "italic red" << reset << " text" << endl;
  o << green << italic << "italic green" << reset << " text" << endl;
  o << yellow << italic << "italic yellow" << reset << " text" << endl;
  o << blue << italic << "italic blue" << reset << " text" << endl;
  o << orange << italic << "italic orange" << reset << " text" << endl;

  o << ok << "ok" << resetfg << " text" << endl;
  o << warn << "warn" << resetfg << " text" << endl;
  o << fail << "fail" << resetfg << " text" << endl;

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
