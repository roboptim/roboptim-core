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
 * \brief Gnuplot visualization.
 */

#ifndef ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# define ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
# include <boost/format.hpp>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      class Command
      {
      public:
	explicit Command (std::string cmd) throw ()
	  : command_ (cmd)
	{}

	~Command () throw () {}

	const std::string& command () const throw ()
	{
	  return command_;
	}
      protected:
	std::string command_;
      };

# define GNUPLOT_UNARY_COMMAND(NAME)		\
      Command					\
      NAME () throw ()				\
      {						\
	return Command (#NAME);			\
      }

# define GNUPLOT_STR_COMMAND(NAME, FARG, ARG)	\
      Command					\
      NAME (const char* FARG) throw ()		\
      {						\
	std::string command = #NAME;		\
	if (*ARG != 0)				\
	  {					\
	    command += " '";			\
	    command += ARG;			\
	    command += "'";			\
	  }					\
	return Command (command);		\
      }

      Command
      comment (const char* content) throw ()
      {
	std::string str = "#";
	str += content;
	return Command (str);
      }

      Command
      set (const char* var, const char* value) throw ()
      {
	return Command ((boost::format ("set %1% %2%") % var % value).str ());
      }

      Command
      show (const char* var, const char* value) throw ()
      {
	return Command ((boost::format ("show %1% %2%") % var % value).str ());
      }

      GNUPLOT_UNARY_COMMAND (clear)
      GNUPLOT_UNARY_COMMAND (pwd)
      GNUPLOT_UNARY_COMMAND (quit)
      GNUPLOT_UNARY_COMMAND (replot)
      GNUPLOT_UNARY_COMMAND (reread)
      GNUPLOT_UNARY_COMMAND (reset)

      GNUPLOT_STR_COMMAND(cd, path, path)
      GNUPLOT_STR_COMMAND(help, topic = "", topic)


# undef GNUPLOT_STR_COMMAND
# undef GNUPLOT_UNARY_COMMAND

    } // end of namespace gnuplot.

  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_CORE_VISUALIZATION_GNUPLOT_COMMANDS_HH
