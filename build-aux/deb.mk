include $(top_srcdir)/build-aux/init.mk

DPKG_BUILDPACKAGE = dpkg-buildpackage -rfakeroot -us -uc -d
distdir = $(PACKAGE)-$(VERSION)
DIST_ARCHIVE = $(distdir).tar.gz

EXTRA_DIST +=                                   \
        changelog                               \
        compat                                  \
        control                                 \
        copyright                               \
	docs					\
	rules

.PHONY: deb

deb: $(top_builddir)/$(DIST_ARCHIVE)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) dist-gzip
	rm -rf $(distdir)
	tar xvzf $<
	ln -s packages/debian $(distdir)
	cd $(distdir) && $(DPKG_BUILDPACKAGE)
