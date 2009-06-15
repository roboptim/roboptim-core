include $(top_srcdir)/build-aux/init.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

lib_LTLIBRARIES = libroboptim-core.la 		\
		  roboptim-core-dummy-plugin.la

libroboptim_core_la_SOURCES = 		\
	constant-function.cc		\
	doc.hh				\
	derivable-function.cc		\
	finite-difference-gradient.cc	\
	function.cc			\
	generic-solver.cc		\
	identity-function.cc		\
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
	visualization/gnuplot.cc	\
	visualization/gnuplot-commands.cc

libroboptim_core_la_LDFLAGS = -version-info 1:1:0

roboptim_core_dummy_plugin_la_SOURCES = roboptim-core-dummy-plugin.cc
roboptim_core_dummy_plugin_la_LDFLAGS = -module -version-info 1:1:0
