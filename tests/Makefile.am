include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/build-aux/autotest.mk

# Add available warnings flags.
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

# Package the following files.
EXTRA_DIST += package.m4.in

# Clean atconfig (do not distribute it).
CLEANFILES += atconfig

# Source which are shared by all tests.
COMMON_SOURCES = common.hh

# simple
# Link the dummy solver with this test to bypass the plug-in system.
check_PROGRAMS = simple
simple_SOURCES = simple.cc $(top_builddir)/src/roboptim-core-dummy-plugin.cc \
		$(COMMON_SOURCES)
simple_LDADD = $(top_builddir)/src/libroboptim-core.la

# interval
check_PROGRAMS += interval
interval_SOURCES = interval.cc $(COMMON_SOURCES)
interval_LDADD = $(top_builddir)/src/libroboptim-core.la

# util
check_PROGRAMS += util
util_SOURCES = util.cc $(COMMON_SOURCES)
util_LDADD = $(top_builddir)/src/libroboptim-core.la

# result
check_PROGRAMS += result
result_SOURCES = result.cc $(COMMON_SOURCES)
result_LDADD = $(top_builddir)/src/libroboptim-core.la

# function
check_PROGRAMS += function
function_SOURCES = function.cc $(COMMON_SOURCES)
function_LDADD = $(top_builddir)/src/libroboptim-core.la

# derivable-function
check_PROGRAMS += derivable-function
derivable_function_SOURCES = derivable-function.cc $(COMMON_SOURCES)
derivable_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# twice-derivable-function
check_PROGRAMS += twice-derivable-function
twice_derivable_function_SOURCES = twice-derivable-function.cc $(COMMON_SOURCES)
twice_derivable_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# quadratic-function
check_PROGRAMS += quadratic-function
quadratic_function_SOURCES = quadratic-function.cc $(COMMON_SOURCES)
quadratic_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# linear-function
check_PROGRAMS += linear-function
linear_function_SOURCES = linear-function.cc $(COMMON_SOURCES)
linear_function_LDADD = $(top_builddir)/src/libroboptim-core.la

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

# parametrized-function
check_PROGRAMS += parametrized-function
parametrized_function_SOURCES = parametrized-function.cc \
	$(COMMON_SOURCES)
parametrized_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# derivable-parametrized-function
check_PROGRAMS += derivable-parametrized-function
derivable_parametrized_function_SOURCES = derivable-parametrized-function.cc \
	$(COMMON_SOURCES)
derivable_parametrized_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# plugin
check_PROGRAMS += plugin
plugin_SOURCES = plugin.cc $(COMMON_SOURCES)
plugin_LDADD = $(top_builddir)/src/libroboptim-core.la

# finite-difference-gradient
check_PROGRAMS += finite-difference-gradient
finite_difference_gradient_SOURCES = finite-difference-gradient.cc \
					$(COMMON_SOURCES)
finite_difference_gradient_LDADD = $(top_builddir)/src/libroboptim-core.la

# identity-function
check_PROGRAMS += identity-function
identity_function_SOURCES = identity-function.cc $(COMMON_SOURCES)
identity_function_LDADD = $(top_builddir)/src/libroboptim-core.la

# constant-function
check_PROGRAMS += constant-function
constant_function_SOURCES = constant-function.cc $(COMMON_SOURCES)
constant_function_LDADD = $(top_builddir)/src/libroboptim-core.la

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
EXTRA_DIST +=						\
	constant-function.stdout			\
	derivable-function.stdout			\
	finite-difference-gradient.stdout		\
	function.stdout					\
	identity-function.stdout			\
	interval.stdout					\
	linear-function.stdout				\
	n-times-derivable-function.stdout		\
	numeric-linear-function.stdout			\
	numeric-quadratic-function.stdout		\
	quadratic-function.stdout			\
	parametrized-function.stdout			\
	derivable-parametrized-function.stdout		\
	plugin.stdout					\
	problem-cc.stdout				\
	result.stdout					\
	simple.stdout					\
	twice-derivable-function.stdout			\
	util.stdout					\
	visualization-gnuplot-function.stdout		\
	visualization-gnuplot-simple.stdout
