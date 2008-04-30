include ../Makefile.conf

all:  makedisc

makedisc: makedisc.o
	$(CC) $^ -o $@ 

makedisc.o: makedisc.c $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

disc:
	$(bindir)/makedisc 64 32 30000 260000 83000

clean:
	rm -f *.o *~ makedisc

install: makedisc
	mkdir -p $(DESTDIR)/bin
	install -s -m 755 makedisc $(DESTDIR)/bin
