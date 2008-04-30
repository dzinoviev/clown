include ../Makefile.conf

all:  clodd

clodd: clodd.o options.o $(libdir)/libdevices.a
	$(CC) $^ -o $@ -L$(libdir) -ldevices

clodd.o: clodd.c clodd.h $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

options.o: options.c clodd.h $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

clean:
	rm -f *.o *~ clodd

install: clodd
	mkdir -p $(DESTDIR)/bin
	install -s -m 755 clodd $(DESTDIR)/bin

