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
simple_LDADD = $(top_builddir)/src/liboptimization.la

# problem-cc
check_PROGRAMS += problem-cc
problem_cc_SOURCES = problem-cc.cc $(COMMON_SOURCES)
problem_cc_LDADD = $(top_builddir)/src/liboptimization.la

# numeric-linear-function
check_PROGRAMS += numeric-linear-function
numeric_linear_function_SOURCES = numeric-linear-function.cc $(COMMON_SOURCES)
numeric_linear_function_LDADD = $(top_builddir)/src/liboptimization.la

# numeric-quadratic-function
check_PROGRAMS += numeric-quadratic-function
numeric_quadratic_function_SOURCES = numeric-quadratic-function.cc \
	$(COMMON_SOURCES)
numeric_quadratic_function_LDADD = $(top_builddir)/src/liboptimization.la


# Ipopt
if HAVE_IPOPT
# Do not call the binary ipopt as Ipopt might also install a binary
# with that name.
check_PROGRAMS += ipopt-test
ipopt_test_SOURCES = ipopt.cc $(COMMON_SOURCES)
ipopt_test_LDADD = $(top_builddir)/src/liboptimization.la
endif


# CFSQP
if HAVE_CFSQP
check_PROGRAMS += cfsqp
cfsqp_SOURCES = cfsqp.cc $(COMMON_SOURCES)
cfsqp_LDADD = $(top_builddir)/src/liboptimization.la
endif



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
	ipopt-test.stdout			\
	cfsqp.stdout				\
	numeric-linear-function.stdout		\
	numeric-quadratic-function.stdout	\
	problem-cc.stdout
