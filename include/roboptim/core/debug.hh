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
# include <log4cxx/logger.h>
# include <roboptim/core/indent.hh>

# define RoboptimCoreDebug(STATEMENT)		\
  LOG4CXX_DEBUG(log4cxx::Logger::getLogger("roboptim.core"),	\
		STATEMENT)

#  define RoboptimCoreDout(cntrl, data)				\
  LOG4CXX_INFO(log4cxx::Logger::getLogger("roboptim.core"),	\
	       data)

#define RoboptimCoreDoutFatal(cntrl, data)			\
  LOG4CXX_INFO(log4cxx::Logger::getLogger("roboptim.core"),	\
	       data)

#define RoboptimCoreForAllDebugChannels(STATEMENT)			\
  LOG4CXX_INFO(log4cxx::Logger::getLogger("roboptim.core"),		\
	       STATEMEMT)
#define RoboptimCoreForAllDebugObjects(STATEMENT)		\
  LOG4CXX_INFO(log4cxx::Logger::getLogger("roboptim.core"),	\
	       STATEMENT)

# ifdef NDEBUG
#  define ROBOPTIM_DEBUG_ONLY(X)
# else
#  define ROBOPTIM_DEBUG_ONLY(X) X
# endif // NDEBUG

#endif // !ROBOPTIM_CORE_DEBUG_HH
