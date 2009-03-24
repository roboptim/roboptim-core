include $(top_srcdir)/build-aux/init.mk

SUBDIRS = plug-in

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

lib_LTLIBRARIES = liboptimization.la

include_HEADERS =					\
				optimization-fwd.hh	\
				optimization.hh

liboptimization_la_SOURCES = 				\
				doc.hh			\
				optimization.cc
