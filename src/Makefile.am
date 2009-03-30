include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

include_HEADERS =					\
				optimization-fwd.hh	\
				solver.hh		\
				dummy.hh		\
				ipopt.hh		\
				cfsqp.hh

lib_LTLIBRARIES = liboptimization.la

liboptimization_la_SOURCES = 				\
				doc.hh			\
				solver.cc		\
				dummy.cc		\
				ipopt.cc		\
				cfsqp.cc

liboptimization_la_LIBADD = -lipopt -lgfortran
liboptimization_la_LDFLAGS = -pthread
