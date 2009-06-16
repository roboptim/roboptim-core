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


# Nothing to do in this directory.
all:

# Libtool rules
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

include $(top_srcdir)/build-aux/pkg-config.mk

# -- Generate documentation
.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@
