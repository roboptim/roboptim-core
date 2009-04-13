include $(top_srcdir)/build-aux/init.mk

# --- Distribute and install headers.
nobase_include_HEADERS =				\
	liboptimization/derivable-function.hh		\
	liboptimization/dummy.hh			\
	liboptimization/function.hh			\
	liboptimization/fwd.hh				\
	liboptimization/generic-solver.hh		\
	liboptimization/indent.hh			\
	liboptimization/linear-function.hh		\
	liboptimization/numeric-quadratic-function.hh	\
	liboptimization/numeric-linear-function.hh	\
	liboptimization/problem.hh			\
	liboptimization/problem.hxx			\
	liboptimization/quadratic-function.hh		\
	liboptimization/result.hh			\
	liboptimization/result-with-warnings.hh		\
	liboptimization/solver-error.hh			\
	liboptimization/solver-warning.hh		\
	liboptimization/solver.hh			\
	liboptimization/solver.hxx			\
	liboptimization/twice-derivable-function.hh	\
	liboptimization/util.hh				\
	liboptimization/util.hxx

if HAVE_CFSQP
 nobase_include_HEADERS += liboptimization/cfsqp.hh
endif

if HAVE_IPOPT
 nobase_include_HEADERS += liboptimization/ipopt.hh
endif
