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

#include <stdexcept>
#include "roboptim/core/config.hh"

#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <roboptim/core/visualization/gnuplot-commands.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      Command::Command (std::string cmd)
	: command_ (cmd)
      {}

      Command::~Command ()
      {}

      const std::string&
      Command::command () const
      {
	return command_;
      }


# define GNUPLOT_UNARY_COMMAND(NAME)		\
      Command					\
      NAME ()					\
      {						\
	return Command (#NAME);			\
      }

# define GNUPLOT_STR_COMMAND(NAME, ARG)		\
      Command					\
      NAME (const char* ARG)			\
      {						\
	std::string command = #NAME;		\
	  if (*ARG != 0)			\
	    {					\
	      command += " '";			\
	      command += ARG;			\
	      command += "'";			\
	    }					\
	  return Command (command);		\
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
        if (!value || !*value)
	  {
	    boost::format fmt = boost::format ("set %1%");
	    // Ignore some irrelevant exceptions
	    fmt.exceptions (boost::io::all_error_bits
			    ^ (boost::io::too_many_args_bit
			       | boost::io::too_few_args_bit
			       | boost::io::bad_format_string_bit));
	    return Command ((fmt % var).str ());
	  }
        boost::format fmt = boost::format ("set %1% %2%");
	// Ignore some irrelevant exceptions
        fmt.exceptions (boost::io::all_error_bits
                        ^ (boost::io::too_many_args_bit
                           | boost::io::too_few_args_bit
                           | boost::io::bad_format_string_bit));
        return Command ((fmt % var % value).str ());
      }

      Command
      unset (const char* var)
      {
        boost::format fmt ("unset %1%");
        // Ignore some irrelevant exceptions
        fmt.exceptions (boost::io::all_error_bits
                        ^ (boost::io::too_many_args_bit
                           | boost::io::too_few_args_bit
                           | boost::io::bad_format_string_bit));
        return Command ((fmt % var).str ());
      }

      Command
      show (const char* var)
      {
        boost::format fmt ("show %1%");
        // Ignore some irrelevant exceptions
        fmt.exceptions (boost::io::all_error_bits
                        ^ (boost::io::too_many_args_bit
                           | boost::io::too_few_args_bit
                           | boost::io::bad_format_string_bit));
        return Command ((fmt % var).str ());
      }

      GNUPLOT_UNARY_COMMAND (clear)
      GNUPLOT_UNARY_COMMAND (pwd)
      GNUPLOT_UNARY_COMMAND (quit)
      GNUPLOT_UNARY_COMMAND (replot)
      GNUPLOT_UNARY_COMMAND (reread)
      GNUPLOT_UNARY_COMMAND (reset)

      GNUPLOT_STR_COMMAND (cd, path)
      GNUPLOT_STR_COMMAND (help, topic)

# undef GNUPLOT_STR_COMMAND
# undef GNUPLOT_UNARY_COMMAND

    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim
