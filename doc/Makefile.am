include $(top_srcdir)/build-aux/init.mk

# Variables.
EXTRA_DIST += 				\
	Doxyfile.in 			\
	header.html 			\
	footer.html 			\
	package.css 			\
	doxygen.css			\
	tabs.css			\
	sf-upload.sh.in

CLEANFILES +=				\
	$(PACKAGE_TARNAME).doxytag 	\
	Doxyfile			\
	config.log


.PHONY: doc				\
	html				\
	doxygen 			\
	install-doxygen-html		\
	uninstall-doxygen-html

# Rules.
doc: html

html-local: doxygen

doxygen: Doxyfile
	$(DOXYGEN) "$<"

# Doxygen generation rule.
Doxyfile: $(srcdir)/Doxyfile.in
	$(top_builddir)/config.status --file="$@":"$<" \
	&& sed -i -e 's/^#.*//' -e '/^$$/d' "$@"

# Clean rule.
clean-local:
	rm -rf html latex

# Install rules.
install-data-local: install-doxygen-html
uninstall-local: uninstall-doxygen-html

install-doxygen-html: html-local
	$(mkdir_p) "$(DESTDIR)$(htmldir)"
	cp -rf html "$(DESTDIR)$(htmldir)"
	$(INSTALL_DATA) $(PACKAGE_TARNAME).doxytag "$(DESTDIR)$(htmldir)/html"

uninstall-doxygen-html:
	rm -rf "$(DESTDIR)$(htmldir)"

# --- SourceForge upload script.
# sf-upload.sh generation rule.
sf-upload.sh: $(srcdir)/sf-upload.sh.in
	$(top_builddir)/config.status --file="$@":"$<" && chmod 755 "$@"
