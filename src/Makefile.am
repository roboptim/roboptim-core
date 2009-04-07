include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

include_HEADERS =					\
				fwd.hh			\
				solver.hh		\
				dummy.hh		\
				function.hh		\
				util.hh

lib_LTLIBRARIES = liboptimization.la

liboptimization_la_SOURCES = 				\
				doc.hh			\
				solver.cc		\
				dummy.cc		\
				function.cc		\
				util.cc

liboptimization_la_LIBADD =
liboptimization_la_LDFLAGS =
liboptimization_la_CXXFLAGS =

if IPOPT
 include_HEADERS += ipopt.hh
 liboptimization_la_SOURCES += ipopt.cc
 liboptimization_la_CXXFLAGS += $(PTHREAD_CFLAGS)
 liboptimization_la_LDFLAGS += $(IPOPT_LDFLAGS)
 liboptimization_la_LIBADD += $(IPOPT_LIBADD) $(PTHREAD_LIBS)
endif

if CFSQP
 include_HEADERS += cfsqp.hh
 liboptimization_la_SOURCES += cfsqp.cc
 liboptimization_la_LIBADD += $(CFSQP_LIBADD)
endif
