include $(top_srcdir)/build-aux/init.mk

DPKG_BUILDPACKAGE = dpkg-buildpackage -rfakeroot -E -W -d
distdir = $(PACKAGE)-$(VERSION)
DIST_ARCHIVE = $(distdir).tar.gz

EXTRA_DIST +=                                   \
        changelog                               \
        compat                                  \
        control                                 \
        copyright                               \
	docs					\
	rules

.PHONY: deb deb-prepare

clean-local:
	rm -rf $(PACKAGE)-$(VERSION)
	rm -f $(DIST_ARCHIVE)
	rm -f $(PACKAGE)_$(VERSION)-*.tar.gz
	rm -f $(PACKAGE)_$(VERSION)-*.dsc
	rm -f $(PACKAGE)_$(VERSION)-*.changes
	rm -f $(PACKAGE)_$(VERSION)-*.deb

deb-prepare:
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) dist-gzip
	rm -rf $(distdir)
	tar xzf $(top_builddir)/$(DIST_ARCHIVE)
	ln -s packages/debian $(distdir)

deb: deb-prepare
	cd $(distdir) && $(DPKG_BUILDPACKAGE)
