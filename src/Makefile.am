include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

include_HEADERS =					\
				fwd.hh			\
				solver.hh		\
				dummy.hh		\
				ipopt.hh		\
				cfsqp.hh		\
				function.hh		\
				problem.hh		\
				util.hh

lib_LTLIBRARIES = liboptimization.la

liboptimization_la_SOURCES = 				\
				doc.hh			\
				solver.cc		\
				dummy.cc		\
				ipopt.cc		\
				cfsqp.cc		\
				function.cc		\
				problem.cc		\
				util.cc

liboptimization_la_LIBADD = -lipopt -lgfortran -lcfsqp
liboptimization_la_LDFLAGS = -pthread
