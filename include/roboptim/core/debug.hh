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
# include <sstream>

# include <boost/config.hpp>
# include <boost/current_function.hpp>

# include <log4cxx/logger.h>
# include <roboptim/core/indent.hh>

// Note: the API is inspired by BOOST_ASSERT. We reimplement the high-level
// macros to prevent any clash with the user-defined assert handlers that live
// in the boost namespace.
namespace roboptim
{
  /// \brief Handler called when an insertion fails.
  ///
  /// \param expr expression used for the insertion.
  /// \param function function where the assertion was made.
  /// \param file file where the assertion was made.
  /// \param line line where the assertion was made.
  ROBOPTIM_DLLEXPORT
  void assertion_failed(char const *expr, char const *function, char const *file,
                        long line);

  /// \brief Handler called when an insertion fails, with an extra error
  /// message.
  ///
  /// \param expr expression used for the insertion.
  /// \param msg
  /// \param function function where the assertion was made.
  /// \param file file where the assertion was made.
  /// \param line line where the assertion was made.
  ROBOPTIM_DLLEXPORT
  void assertion_failed_msg(char const *expr, char const *msg,
                            char const *function, char const *file, long line);
}

#ifdef NDEBUG
#  define ROBOPTIM_ASSERT_INTERNAL(expr) ((void)0)
#  define ROBOPTIM_ASSERT_MSG_INTERNAL(expr, msg) ((void)0)
#else // NDEBUG
#  define ROBOPTIM_ASSERT_INTERNAL(expr)                                     \
    (BOOST_LIKELY(!!(expr)) ? ((void)0) : ::roboptim::assertion_failed(      \
                                              #expr, BOOST_CURRENT_FUNCTION, \
                                              __FILE__, __LINE__))
#  define ROBOPTIM_ASSERT_MSG_INTERNAL(expr, msg)                           \
    (BOOST_LIKELY(!!(expr))                                                 \
         ? ((void)0)                                                        \
         : ::roboptim::assertion_failed_msg(                                \
               #expr,                                                       \
               ((std::ostringstream &)(std::stringstream().flush() << msg)) \
                   .str()                                                   \
                   .c_str(),                                                \
               BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#endif // NDEBUG

#define ROBOPTIM_ASSERT(expr) ROBOPTIM_ASSERT_INTERNAL(expr)
#define ROBOPTIM_ASSERT_MSG(expr,msg) ROBOPTIM_ASSERT_MSG_INTERNAL(expr, msg)

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
