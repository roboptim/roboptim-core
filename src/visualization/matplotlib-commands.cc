// Copyright (C) 2014 by Benjamin Chrétien, CNRS-LIRMM.
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

#include <stdexcept>
#include "roboptim/core/config.hh"

#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <roboptim/core/visualization/matplotlib-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace matplotlib
    {
      Import::Import (const std::string& packages)
        : command_ ("import " + packages)
      {}

      Import::Import (const std::string& from, const std::string& packages)
        : command_ ("from " + from + " import " + packages)
      {}

      Import::~Import ()
      {}

      Command::Command (const std::string& cmd)
	: command_ (cmd)
      {}

      Command::~Command ()
      {}

      const std::string&
      Command::command () const
      {
	return command_;
      }


# define MATPLOTLIB_UNARY_COMMAND(NAME)				\
      Command							\
      NAME ()							\
      {								\
	return Command ("plt." BOOST_PP_STRINGIZE(NAME) " ()");	\
      }

      Command
      comment (const char* content)
      {
	using namespace boost::xpressive;

	// Comment character
	std::string comment_char = "# ";

	// Newlines should be prefixed by #
	sregex pattern = sregex::compile ("(\n)");
	std::string format ("$1" + comment_char);

	return Command (comment_char +
			regex_replace (std::string (content), pattern, format));
      }

      Command
      set (const char* var, const char* value)
      {
        if (!*value)
	  {
	    throw std::runtime_error ("matplotlib::set (var, value) expects an "
				      "actual value.");
	  }

	boost::format fmt = boost::format ("%1% = %2%");

	// Ignore some irrelevant exceptions
	fmt.exceptions (boost::io::all_error_bits
			^ (boost::io::too_many_args_bit
			   | boost::io::too_few_args_bit
			   | boost::io::bad_format_string_bit));

	return Command ((fmt % var % value).str ());
      }

      MATPLOTLIB_UNARY_COMMAND (show)
      MATPLOTLIB_UNARY_COMMAND (figure)

# undef MATPLOTLIB_UNARY_COMMAND

    } // end of namespace matplotlib.
  } // end of namespace visualization.
} // end of namespace roboptim
