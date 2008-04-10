INCLUDE = ../include/
CFLAGS = -Wall -ansi  -g -I$(INCLUDE) -O3
DEVICES = ucvm.o tty.o timer.o hdd.o hddlib.o dma.o

all:	libdevices.a

libdevices.a: $(DEVICES)
	ar -rcs libdevices.a $(DEVICES)

tty.o: tty.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/options.h

timer.o: timer.c  $(INCLUDE)/clown_types.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h

hdd.o:   hdd.c   $(INCLUDE)/clown_types.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h $(INCLUDE)/hdd.h

hddlib.o:  hddlib.c $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/registers.h $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h

dma.o:   dma.c   $(INCLUDE)/clown_types.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h $(INCLUDE)/hdd.h $(INCLUDE)/config.h $(INCLUDE)/prototypes.h $(INCLUDE)/ucvm.h 

ucvm.o: ucvm.c $(INCLUDE)/ucvm.h $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h 

clean:
	rm -f clown $(DEVICES) *~

cleanall:
	rm -f clown $(DEVICES) *~ libdevices.a
