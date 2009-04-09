include $(top_srcdir)/build-aux/init.mk

# --- Distribute and install headers.
nobase_include_HEADERS =				\
	liboptimization/derivable-function.hh		\
	liboptimization/dummy.hh			\
	liboptimization/function.hh			\
	liboptimization/fwd.hh				\
	liboptimization/generic-solver.hh		\
	liboptimization/linear-function.hh		\
	liboptimization/problem.hh			\
	liboptimization/problem.hxx			\
	liboptimization/problem-converter.hh		\
	liboptimization/problem-converter.hxx		\
	liboptimization/quadratic-function.hh		\
	liboptimization/solver-error.hh			\
	liboptimization/solver.hh			\
	liboptimization/solver.hxx			\
	liboptimization/twice-derivable-function.hh	\
	liboptimization/util.hh				\
	liboptimization/util.hxx

if CFSQP
 nobase_include_HEADERS += liboptimization/cfsqp.hh
endif

if IPOPT
 nobase_include_HEADERS += liboptimization/ipopt.hh
endif
