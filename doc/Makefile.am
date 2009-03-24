DOXYGEN = doxygen

.PHONY: doc html install-doxygen-html uninstall-doxygen-html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

edit = sed -e "s|@ID@|$$Id|" \
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST = Doxyfile.in
CLEANFILES = Doxyfile

# Use sed to generate Doxyfile.
Doxyfile: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile.in
	Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; \
	$(edit) $(srcdir)/Doxyfile.in >Doxyfile

clean-local:
	rm -rf html latex

install-data-local: install-doxygen-html
uninstall-local: uninstall-doxygen-html

install-doxygen-html: html-local
	$(MKDIR_P) "$(DESTDIR)$(htmldir)"
	cp -rf html/* "$(DESTDIR)$(htmldir)"
	cd "$(DESTDIR)$(htmldir)" && ./installdox -q

uninstall-doxygen-html:
	rm -rf "$(DESTDIR)$(htmldir)"
