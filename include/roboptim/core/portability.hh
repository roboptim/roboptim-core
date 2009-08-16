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
# ifdef _WIN32
#  define ROBOPTIM_DLLIMPORT __declspec(dllimport)
#  define ROBOPTIM_DLLEXPORT __declspec(dllexport)
# else
#  define ROBOPTIM_DLLIMPORT
#  define ROBOPTIM_DLLEXPORT
# endif //! WIN32

# ifndef BUILDING_ROBOPTIM
#  define ROBOPTIM_DLLAPI ROBOPTIM_DLLIMPORT
# else
#  define ROBOPTIM_DLLAPI ROBOPTIM_DLLEXPORT
# endif //! BUILDING_ROBOPTIM_CORE

#endif //! ROBOPTIM_CORE_PORTABILITY_HH
