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

// Note: DLL macros are defined in config.hh as ROBOPTIM_CORE_DLL*, and were
// defined here as ROBOPTIM_DLL*. We renamed them in roboptim-core, and include
// config.hh for backwards compatibility.
# include <roboptim/core/config.hh>

// Extra useful attributes
#if defined _WIN32 || defined __CYGWIN__
  #define ROBOPTIM_UNUSED __declspec(deprecated)
#else
// On Linux, for GCC >= 4
  #if __GNUC__ >= 4
    #define ROBOPTIM_UNUSED __attribute__((unused))
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

  // TODO: update for Windows if relevant
  #define ROBOPTIM_ALLOW_ATTRIBUTES_ON
  #define ROBOPTIM_ALLOW_ATTRIBUTES_OFF
#elif !defined (__CUDACC__)
// Otherwise (on Linux/OSX with GCC/Clang)
  #define ROBOPTIM_ALLOW_DEPRECATED_ON \
    /* Disable deprecated warning */   \
    _Pragma ("GCC diagnostic push")    \
    _Pragma ("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

  #define ROBOPTIM_ALLOW_DEPRECATED_OFF \
    /* Re-enable deprecated warning */  \
    _Pragma ("GCC diagnostic pop")

  #ifdef __GNUC__
    #define ROBOPTIM_ALLOW_ATTRIBUTES_ON \
    /* Disable attributes warning */     \
    _Pragma ("GCC diagnostic push")      \
    _Pragma ("GCC diagnostic ignored \"-Wattributes\"")

    #define ROBOPTIM_ALLOW_ATTRIBUTES_OFF \
    /* Re-enable attributes warning */    \
    _Pragma ("GCC diagnostic pop")
  #else
    #define ROBOPTIM_ALLOW_ATTRIBUTES_ON
    #define ROBOPTIM_ALLOW_ATTRIBUTES_OFF
  #endif
#else
    #define ROBOPTIM_ALLOW_DEPRECATED_ON
    #define ROBOPTIM_ALLOW_DEPRECATED_OFF
    #define ROBOPTIM_ALLOW_ATTRIBUTES_ON
    #define ROBOPTIM_ALLOW_ATTRIBUTES_OFF
#endif

// Required to avoid size_t resolution error with MSVC. Triggered by
// the boost/tuple/tuple_io.hpp inclusion in roboptim/core/io.hh.
#ifdef _WIN32
# define BOOST_NO_STD_LOCALE
#endif

// Work around for explicit template instantation in GCC, see #111 for
// more details
# if defined(ROBOPTIM_PRECOMPILED_DENSE_SPARSE) && \
  __GNUC__ >= 4 && not defined(__clang__)
# define ROBOPTIM_GCC_ETI_WORKAROUND ROBOPTIM_CORE_DLLAPI
# else
# define ROBOPTIM_GCC_ETI_WORKAROUND
# endif

#endif //! ROBOPTIM_CORE_PORTABILITY_HH
