include $(top_srcdir)/build-aux/init.mk

# ------- #
# CAUTION #
# ------- #

# Currently, I do not know how to build rpms *locally*.
# You will have to know what is your RPM building directory
# (see rpmbuild documentation).
# Set RPM_TREE to this directory. You will need write access
# to this directory.
# Hint: on Gentoo RPM_TREE is ``/usrc/src/rpm'' and on Red Hat
#       ``/usr/src/redhat''.

RPMBUILD = rpmbuild

SPECFILE = $(PACKAGE).spec
EXTRA_DIST += $(SPECFILE)

.PHONY: install-rpm install-rpm-tree install-rpm-tarball src-rpm rpm

# Do nothing by default.
all:

install-rpm: install-rpm-tree install-rpm-tarball

# Prepare tree.
install-rpm-tree:
	@if test "x$$RPM_TREE" = "x"; then	\
	  echo "Please define RPM_TREE.";	\
          exit 1;				\
	fi
	$(mkdir_p) $$RPM_TREE/BUILD
	$(mkdir_p) $$RPM_TREE/RPMS
	$(mkdir_p) $$RPM_TREE/SOURCES
	$(mkdir_p) $$RPM_TREE/SPECS
	$(mkdir_p) $$RPM_TREE/SRPMS

install-rpm-tarball:
	@if test "x$$RPM_TREE" = "x"; then	\
	  echo "Please define RPM_TREE.";	\
          exit 1;				\
	fi
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) dist-gzip
	cp $(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz $$RPM_TREE/SOURCES

# Build source RPM
src-rpm: install-rpm $(srcdir)/$(SPECFILE)
	$(RPMBUILD) -bs $(srcdir)/$(SPECFILE)

# Build binary RPM
rpm: install-rpm $(srcdir)/$(SPECFILE)
	$(RPMBUILD) -bb $(srcdir)/$(SPECFILE)
