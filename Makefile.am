# Search for autoconf macros in build-aux.
ACLOCAL_AMFLAGS = -I build-aux

SUBDIRS = 		\
	doc		\
	packages	\
	include		\
	src		\
	tests


# Provide bootstrap with the generated package.
EXTRA_DIST = bootstrap $(PACKAGE_TARNAME).m4


# Nothing to do in this directory.
all:


# Pkg-config generation
PKG_CONFIG_FILE = @PACKAGE_TARNAME@.pc
PKG_DIR="$(DESTDIR)$(libdir)/pkgconfig/"

edit = sed -e "s|@prefix\@|$(prefix)|" \
           -e 's,@exec_prefix\@,$(exec_prefix),g' \
           -e 's,@libdir\@,$(libdir),g' \
           -e 's,@includedir\@,$(includedir),g' \
           -e 's,@datarootdir\@,$(datarootdir),g' \
           -e 's,@PACKAGE\@,$(PACKAGE),g' \
           -e 's,@PACKAGE_TARNAME\@,$(PACKAGE_TARNAME),g' \
           -e 's,@VERSION\@,$(PACKAGE_VERSION),g'

@PACKAGE_TARNAME@.pc: $(srcdir)/build-aux/pkg-config.pc.in
	$(edit) $(srcdir)/build-aux/pkg-config.pc.in >$@

EXTRA_DIST += build-aux/pkg-config.pc.in
CLEANFILES = $(PKG_CONFIG_FILE)

install-data-local: install-pkg-config
uninstall-local: uninstall-pkg-config

install-pkg-config: $(PACKAGE_TARNAME).pc
	$(MKDIR_P) $(PKG_DIR)
	cp "$(PKG_CONFIG_FILE)" $(PKG_DIR)

uninstall-pkg-config:
	rm "$(PKG_DIR)$(PKG_CONFIG_FILE)"


# -- Generate documentation
.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@
