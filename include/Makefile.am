## Process this file with automake to produce Makefile.in

EXTRA_DIST = *.h
pkginclude_HEADERS = hardware.h

install-data-hook:
	cd $(DESTDIR)/$(pkgincludedir) && \
	rm -f config.h && \
	ln hardware.h config.h
