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

#ifndef DEBUG_HH
# define DEBUG_HH
# include <roboptim/core/sys.hh>

# ifndef CWDEBUG
#  include <iostream>
#  include <cstdlib>

#  define AllocTag1(p)
#  define AllocTag2(p, desc)
#  define AllocTag_dynamic_description(p, data)
#  define AllocTag(p, data)
#  define Debug(STATEMENT)
#  define Dout(cntrl, data)
#  define DoutFatal(cntrl, data) LibcwDoutFatal(, , cntrl, data)
#  define ForAllDebugChannels(STATEMENT)
#  define ForAllDebugObjects(STATEMENT)
#  define LibcwDebug(dc_namespace, STATEMENT)
#  define LibcwDout(dc_namespace, d, cntrl, data)
#  define LibcwDoutFatal(dc_namespace, d, cntrl, data) \
  do { ::std::cerr << data << ::std::endl; ::std::exit(EXIT_FAILURE); } while(1)
#  define LibcwdForAllDebugChannels(dc_namespace, STATEMENT)
#  define LibcwdForAllDebugObjects(dc_namespace, STATEMENT)
#  define NEW(x) new x
#  define CWDEBUG_ALLOC 0
#  define CWDEBUG_MAGIC 0
#  define CWDEBUG_LOCATION 0
#  define CWDEBUG_LIBBFD 0
#  define CWDEBUG_DEBUG 0
#  define CWDEBUG_DEBUGOUTPUT 0
#  define CWDEBUG_DEBUGM 0
#  define CWDEBUG_DEBUGT 0
#  define CWDEBUG_MARKER 0

# endif // CWDEBUG

# define ROBOPTIM_CORE_INTERNAL
# include <roboptim/core/debug.hh>
# define DEBUGCHANNELS roboptim::debug::channels
# ifdef CWDEBUG
#  include <libcwd/debug.h>
# endif

#endif // !DEBUG_HH
