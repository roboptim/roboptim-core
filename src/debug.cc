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

#include <roboptim/core/sys.hh>
#include "debug.hh"

#ifdef CWDEBUG

namespace roboptim
{
  namespace debug
  {
    namespace channels
    {
      namespace dc
      {
      } // namespace dc
    } // namespace DEBUGCHANNELS

    /// Initialize debugging code from new threads.
    void init_thread ();

    void init_thread ()
    {
# if LIBCWD_THREAD_SAFE
      ForAllDebugChannels (while (!debugChannel.is_on ()) debugChannel.on ());
# endif // !LIBCWD_THREAD_SAFE

      // Turn on debug output.
      // Only turn on debug output when the environment variable
      // SUPPRESS_DEBUG_OUTPUT is not set.
      Debug(if (!getenv ("SUPPRESS_DEBUG_OUTPUT")) libcw_do.on ());
# if LIBCWD_THREAD_SAFE
      Debug(libcw_do.set_ostream (&std::cerr, &cerr_mutex));

      // Set the thread id in the margin.
      char margin[12];
      sprintf(margin, "%-10lu ", pthread_self ());
      Debug (libcw_do.margin().assign(margin, 11));
# else
      Debug(libcw_do.set_ostream (&std::cerr));
# endif //! LIBCWD_THREAD_SAFE

      // Write a list of all existing debug channels to the default debug device.
      Debug(list_channels_on (libcw_do));
    }

    /// Initialize debugging code from main().
    void init ();

    void init ()
    {
      // You want this, unless you mix streams output with C output.
      // Read  http://gcc.gnu.org/onlinedocs/libstdc++/27_io/howto.html#8 for an explanation.
      // We can't use it, because other code uses printf to write to the console.
      Debug(set_invisible_on ());
      // Cause "memory leaks" ([w]cin, [w]cout and [w]cerr filebuf allocations).
      std::ios::sync_with_stdio (false);
      Debug(set_invisible_off ());

      // This will warn you when you are using header files that do not belong to the
      // shared libcwd object that you linked with.
      Debug(check_configuration());

# if CWDEBUG_ALLOC
      // Remove all current (pre- main) allocations from the Allocated Memory Overview.
      libcwd::make_all_allocations_invisible_except (0);
# endif

      Debug (read_rcfile ());

      init_thread ();
    }

  } // namespace debug
} // namespace myproject

#endif // CWDEBUG
