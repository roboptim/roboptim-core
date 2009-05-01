.PHONY: doc html install-doxygen-html uninstall-doxygen-html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

EXTRA_DIST = Doxyfile.in header.html footer.html \
	package.css doxygen.css tabs.css sf-upload.sh.in

CLEANFILES = $(PACKAGE_TARNAME).doxytag

clean-local:
	rm -rf html latex

install-data-local: install-doxygen-html
uninstall-local: uninstall-doxygen-html

install-doxygen-html: html-local
	$(mkdir_p) "$(DESTDIR)$(htmldir)/html"
	cp -rf html/* "$(DESTDIR)$(htmldir)/html"
	$(INSTALL_DATA) $(PACKAGE_TARNAME).doxytag "$(DESTDIR)$(htmldir)/html"

uninstall-doxygen-html:
	rm -rf "$(DESTDIR)$(htmldir)"
