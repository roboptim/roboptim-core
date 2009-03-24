# Get rid of the annoying Automake behavior which forces `+='
# to be used after `='

AM_CPPFLAGS=
AM_CXXFLAGS=
AM_LDFLAGS=
CLEANFILES=
DISTCLEANFILES=
EXTRA_DIST=
SUFFIXES=

# Add top_builddir to AM_CPPFLAGS in order to find config.h
# automatically.
AM_CPPFLAGS+=-I$(top_builddir)

# Force generated files to be searched *before* source files.
AM_CPPFLAGS += -I$(top_builddir)/src -I$(top_srcdir)/src
