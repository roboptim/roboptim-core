include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/build-aux/autotest.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

# Package the following files.
EXTRA_DIST += package.m4.in

# Clean atconfig (do not distribute it).
CLEANFILES += atconfig

# Source which are shared by all tests.
COMMON_SOURCES = common.hh hs071.hh

# simple
check_PROGRAMS = simple
simple_SOURCES = simple.cc $(COMMON_SOURCES)
simple_LDADD = $(top_builddir)/src/libroboptim-core.la

# problem-cc
check_PROGRAMS += problem-cc
problem_cc_SOURCES = problem-cc.cc $(COMMON_SOURCES)
problem_cc_LDADD = $(top_builddir)/src/libroboptim-core.la

# numeric-linear-function
check_PROGRAMS += numeric-linear-function
numeric_linear_function_SOURCES = numeric-linear-function.cc $(COMMON_SOURCES)
numeric_linear_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# numeric-quadratic-function
check_PROGRAMS += numeric-quadratic-function
numeric_quadratic_function_SOURCES = numeric-quadratic-function.cc \
	$(COMMON_SOURCES)
numeric_quadratic_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# n-times-derivable-function
check_PROGRAMS += n-times-derivable-function
n_times_derivable_function_SOURCES = n-times-derivable-function.cc \
	$(COMMON_SOURCES)
n_times_derivable_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# visualization-gnuplot-simple
check_PROGRAMS += visualization-gnuplot-simple
visualization_gnuplot_simple_SOURCES = visualization-gnuplot-simple.cc \
	$(COMMON_SOURCES)
visualization_gnuplot_simple_LDADD = $(top_builddir)/src/libroboptim-core.la

# visualization-gnuplot-function
check_PROGRAMS += visualization-gnuplot-function
visualization_gnuplot_function_SOURCES = visualization-gnuplot-function.cc \
	$(COMMON_SOURCES)
visualization_gnuplot_function_LDADD = $(top_builddir)/src/libroboptim-core.la


.PHONY: generate-reference

# Re-generate reference outputs.
generate-reference:
	@echo "Generating references...";	\
	for i in $(check_PROGRAMS); do		\
	 IN="$(srcdir)/$$i.stdin";		\
	 OUT="$(srcdir)/$$i.stdout";		\
	 if test -f "$$IN"; then 		\
	  "./$$i" < $$IN > $$OUT || true;	\
	 else					\
	  "./$$i" > $$OUT || true;		\
	 fi					\
	done


# ------------ #
# Test suite.  #
# ------------ #

# Distribute reference files.
EXTRA_DIST +=					\
	simple.stdout				\
	numeric-linear-function.stdout		\
	numeric-quadratic-function.stdout	\
	n-times-derivable-function.stdout	\
	problem-cc.stdout			\
	visualization-gnuplot-simple.stdout	\
	visualization-gnuplot-function.stdout
