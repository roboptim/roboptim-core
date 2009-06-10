include $(top_srcdir)/build-aux/init.mk

# Search for autoconf macros in build-aux.
ACLOCAL_AMFLAGS = -I build-aux

SUBDIRS = 		\
	doc		\
	include		\
	src		\
	tests


# Provide bootstrap with the generated package.
EXTRA_DIST = bootstrap build-aux/bootstrap $(PACKAGE_TARNAME).m4 THANKS TODO


# Nothing to do in this directory.
all:

# Libtool rules
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# Pkg-config generation
PKG_CONFIG_FILE = @PACKAGE_TARNAME@.pc
PKG_DIR="$(DESTDIR)$(libdir)/pkgconfig/"

edit = $(SED) -e "s|@prefix\@|$(prefix)|"			\
              -e 's,@exec_prefix\@,$(exec_prefix),g'		\
              -e 's,@libdir\@,$(libdir),g'			\
              -e 's,@includedir\@,$(includedir),g'		\
              -e 's,@datarootdir\@,$(datarootdir),g'		\
              -e 's,@PACKAGE\@,$(PACKAGE),g'			\
              -e 's,@PACKAGE_TARNAME\@,$(PACKAGE_TARNAME),g'	\
              -e 's,@VERSION\@,$(PACKAGE_VERSION),g'

include $(top_srcdir)/build-aux/pkg-config.mk


# -- Generate documentation
.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@
