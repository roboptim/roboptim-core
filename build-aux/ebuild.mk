include $(top_srcdir)/build-aux/init.mk

# Distribute the files required to build the ebuilds.
EXTRA_DIST +=						\
		ChangeLog				\
		metadata.xml				\
		$(PACKAGE)-$(PACKAGE_VERSION).ebuild	\
		$(PACKAGE)-9999.ebuild


.PHONY: check-ebuild install-ebuild

# Install the ebuild into a local portage tree.
# For testing purpose only, this rule has to be called manually.
install-ebuild:
	if test "x$$PORTAGE_ROOT" = "x"; then				 \
	 echo -n "Please define PORTAGE_ROOT ";				 \
	 echo "to the portage root you want to install the ebuild into.";\
	else								 \
         DIR="$$PORTAGE_ROOT/$(EBUILD_CATEGORY)/$(PACKAGE)";		 \
	 $(mkdir_p) $$DIR/files;					 \
	 for i in `echo "$(EXTRA_DIST)" | sed 's/ /\n/g'`; do		 \
	  cp "$(srcdir)/$$i" "$$DIR";					 \
	  if test x`echo "$$i" | grep "\.ebuild$$"` = "x$$i"; then	 \
	   ebuild $$DIR/$$i manifest;					 \
	  fi								 \
	 done								 \
	fi

# Check that the metadata.xml file is valid.
check-ebuild:
	xmllint --valid "$(srcdir)/metadata.xml" > /dev/null
