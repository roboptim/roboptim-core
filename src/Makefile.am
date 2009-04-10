include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

lib_LTLIBRARIES = liboptimization.la

liboptimization_la_SOURCES = 		\
	doc.hh				\
	derivable-function.cc		\
	dummy.cc			\
	function.cc			\
	generic-solver.cc		\
	indent.cc			\
	linear-function.cc		\
	quadratic-function.cc		\
	solver-error.cc			\
	twice-derivable-function.cc	\
	util.cc

liboptimization_la_LIBADD =
liboptimization_la_LDFLAGS =
liboptimization_la_CXXFLAGS =

if IPOPT
 liboptimization_la_SOURCES += ipopt.cc
 liboptimization_la_CXXFLAGS += $(PTHREAD_CFLAGS)
 liboptimization_la_LDFLAGS += $(IPOPT_LDFLAGS)
 liboptimization_la_LIBADD += $(IPOPT_LIBADD) $(PTHREAD_LIBS)
endif

if CFSQP
 liboptimization_la_SOURCES += cfsqp.cc
 liboptimization_la_LIBADD += $(CFSQP_LIBADD)
endif
