INCLUDE = ../include/
DEVICES = ../devlib/
CFLAGS = -Wall -g -I$(INCLUDE) -O3 -ansi #-pedantic

all:  ../bin/clodd

../bin/clodd: clodd.o options.o $(DEVICES)/libdevices.a
	$(CC) $^ -o $@ -L$(DEVICES) -ldevices

clodd.o: clodd.c clodd.h $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

options.o: options.c clodd.h $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

clean:
	rm -f *.o *~

cleanall:
	rm -f ../bin/clodd *.o *~

