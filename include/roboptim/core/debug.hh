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

#ifndef ROBOPTIM_CORE_DEBUG_HH
# define ROBOPTIM_CORE_DEBUG_HH

# ifdef CWDEBUG
#  include <libcwd/libraries_debug.h>

namespace roboptim
{
  namespace debug
  {
    /// Initialize debugging code from main ().
    void init ();
    // Initialize debugging code from new threads.
    void init_thread ();

    namespace channels
    {
      namespace dc
      {
	using namespace libcwd::channels::dc;
      }
    }
  }
}
# endif // CWDEBUG
# define RoboptimCoreDebug(STATEMENT...) \
  LibcwDebug(roboptim::debug::channels, STATEMENT)
# define RoboptimCoreDout(cntrl, data) \
  LibcwDout(roboptim::debug::channels, libcwd::libcw_do, cntrl, data)
# define RoboptimCoreDoutFatal(cntrl, data) \
  LibcwDoutFatal(roboptim::debug::channels, libcwd::libcw_do, cntrl, data)
# define RoboptimCoreForAllDebugChannels(STATEMENT...) \
  LibcwdForAllDebugChannels(roboptim::debug::channels, STATEMENT)
# define RoboptimCoreForAllDebugObjects(STATEMENT...) \
  LibcwdForAllDebugObjects(roboptim::debug::channels, STATEMENT)
# if !defined(Debug) && !defined(ROBOPTIM_CORE_INTERNAL)
#  error The application source file (.cc or .cpp) must use '#include "debug.h"' _before_ including the header file that it includes now, that led to this error.
# endif
#endif // !ROBOPTIM_CORE_DEBUG_HH
