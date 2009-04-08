include $(top_srcdir)/build-aux/init.mk

# --- Distribute and install headers.
nobase_include_HEADERS =		\
	liboptimization/dummy.hh	\
	liboptimization/function.hh	\
	liboptimization/fwd.hh		\
	liboptimization/problem.hh	\
	liboptimization/problem.hxx	\
	liboptimization/solver.hh	\
	liboptimization/solver.hxx	\
	liboptimization/util.hh		\
	liboptimization/util.hxx

if CFSQP
 nobase_include_HEADERS += liboptimization/cfsqp.hh
endif

if IPOPT
 nobase_include_HEADERS += liboptimization/ipopt.hh
endif
