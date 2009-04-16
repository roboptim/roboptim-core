include $(top_srcdir)/build-aux/init.mk

# --- Distribute and install headers.
nobase_include_HEADERS =				\
	roboptim-core/derivable-function.hh		\
	roboptim-core/dummy.hh				\
	roboptim-core/function.hh			\
	roboptim-core/fwd.hh				\
	roboptim-core/generic-solver.hh			\
	roboptim-core/indent.hh				\
	roboptim-core/linear-function.hh		\
	roboptim-core/numeric-quadratic-function.hh	\
	roboptim-core/numeric-linear-function.hh	\
	roboptim-core/problem.hh			\
	roboptim-core/problem.hxx			\
	roboptim-core/quadratic-function.hh		\
	roboptim-core/result.hh				\
	roboptim-core/result-with-warnings.hh		\
	roboptim-core/solver-error.hh			\
	roboptim-core/solver-warning.hh			\
	roboptim-core/solver.hh				\
	roboptim-core/solver.hxx			\
	roboptim-core/twice-derivable-function.hh	\
	roboptim-core/util.hh				\
	roboptim-core/util.hxx

if HAVE_CFSQP
 nobase_include_HEADERS += roboptim-core/cfsqp.hh
endif

if HAVE_IPOPT
 nobase_include_HEADERS += roboptim-core/ipopt.hh
endif
