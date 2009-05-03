include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

lib_LTLIBRARIES = libroboptim-core.la

libroboptim_core_la_SOURCES = 		\
	doc.hh				\
	derivable-function.cc		\
	dummy.cc			\
	function.cc			\
	generic-solver.cc		\
	indent.cc			\
	linear-function.cc		\
	numeric-linear-function.cc	\
	numeric-quadratic-function.cc	\
	quadratic-function.cc		\
	result.cc			\
	result-with-warnings.cc		\
	solver-error.cc			\
	solver-warning.cc		\
	twice-derivable-function.cc	\
	util.cc

libroboptim_core_la_SOURCES += 		\
	visualization/gnuplot.cc
