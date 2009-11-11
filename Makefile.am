include $(top_srcdir)/build-aux/init.mk

# Search for autoconf macros in build-aux.
ACLOCAL_AMFLAGS = -I build-aux

SUBDIRS = 		\
	doc		\
	include		\
	src		\
	tests


# Provide bootstrap with the generated package.
EXTRA_DIST +=	bootstrap			\
		build-aux/bootstrap		\
		$(PACKAGE_TARNAME).m4		\
		THANKS				\
		TODO

# Distribute Windows specific files.
EXTRA_DIST += msvc


# Nothing to do in this directory.
all:

# Include various rules...
include $(top_srcdir)/build-aux/doc-recursive.mk
include $(top_srcdir)/build-aux/libtool.mk
include $(top_srcdir)/build-aux/pkg-config.mk
include $(top_srcdir)/build-aux/git-version-gen.mk

include $(top_srcdir)/build-aux/rpm.mk
