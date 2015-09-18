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

#ifndef ROBOPTIM_CORE_PORTABILITY_HH
# define ROBOPTIM_CORE_PORTABILITY_HH



// Handle portable symbol export.
// Defining manually which symbol should be exported is required
// under Windows whether MinGW or MSVC is used.
//
// The headers then have to be able to work in two different modes:
// - dllexport when one is building the library,
// - dllimport for clients using the library.
//
// On Linux, set the visibility accordingly. If C++ symbol visibility
// is handled by the compiler, see: http://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
// On Microsoft Windows, use dllimport and dllexport to tag symbols.
  #define ROBOPTIM_DLLIMPORT __declspec(dllimport)
  #define ROBOPTIM_DLLEXPORT __declspec(dllexport)
  #define ROBOPTIM_DLLLOCAL
#else
// On Linux, for GCC >= 4, tag symbols using GCC extension.
  #if __GNUC__ >= 4
    #define ROBOPTIM_DLLIMPORT __attribute__ ((visibility("default")))
    #define ROBOPTIM_DLLEXPORT __attribute__ ((visibility("default")))
    #define ROBOPTIM_DLLLOCAL  __attribute__ ((visibility("hidden")))
  #else
// Otherwise (GCC < 4 or another compiler is used), export everything.
    #define ROBOPTIM_DLLIMPORT
    #define ROBOPTIM_DLLEXPORT
    #define ROBOPTIM_DLLLOCAL
#endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef ROBOPTIM_STATIC
// If one is using the library statically, get rid of
// extra information.
  #define ROBOPTIM_DLLAPI
  #define ROBOPTIM_LOCAL
#else
// Depending on whether one is building or using the
// library define DLLAPI to import or export.
  #ifdef BUILDING_ROBOPTIM
    #define ROBOPTIM_DLLAPI ROBOPTIM_DLLEXPORT
  #else
    #define ROBOPTIM_DLLAPI ROBOPTIM_DLLIMPORT
  #endif // BUILDING_ROBOPTIM
  #define ROBOPTIM_LOCAL ROBOPTIM_DLLLOCAL
#endif // ROBOPTIM_STATIC

// Extra useful attributes
#if defined _WIN32 || defined __CYGWIN__
  #define ROBOPTIM_UNUSED __declspec(deprecated)
#else
// On Linux, for GCC >= 4
  #if __GNUC__ >= 4
    #define ROBOPTIM_UNUSED __attribute__ ((__unused__))
  #else
// Otherwise (GCC < 4 or another compiler is used)
    #define ROBOPTIM_UNUSED
#endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

// On Microsoft Windows
#if defined _WIN32 || defined __CYGWIN__
  #define ROBOPTIM_ALLOW_DEPRECATED_ON  \
    /* Disable deprecated warning */    \
    __pragma(warning(push))             \
    __pragma(warning(disable : 4996))

  #define ROBOPTIM_ALLOW_DEPRECATED_OFF \
    /* Re-enable deprecated warning */  \
    __pragma(warning(pop))
#else
// Otherwise (on Linux/OSX with GCC/Clang)
  #define ROBOPTIM_ALLOW_DEPRECATED_ON \
    /* Disable deprecated warning */   \
    _Pragma ("GCC diagnostic push")    \
    _Pragma ("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

  #define ROBOPTIM_ALLOW_DEPRECATED_OFF \
    /* Re-enable deprecated warning */  \
    _Pragma ("GCC diagnostic pop")
#endif

// Required to avoid size_t resolution error with MSVC. Triggered by
// the boost/tuple/tuple_io.hpp inclusion in roboptim/core/io.hh.
#ifdef _WIN32
# define BOOST_NO_STD_LOCALE
#endif

#endif //! ROBOPTIM_CORE_PORTABILITY_HH
