include ../Makefile.conf

DEVICES = ucvm.o tty.o timer.o hdd.o hddlib.o dma.o

all:	$(libdir)/libdevices.a

$(libdir)/libdevices.a: $(DEVICES)
	ar -rcs $(libdir)/libdevices.a $(DEVICES)

tty.o: tty.c $(INCLUDE)/options.h $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

timer.o: timer.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

hdd.o:  hdd.c $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

hddlib.o: hddlib.c $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/registers.h \
  $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h

dma.o: dma.c $(INCLUDE)/ucvm.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/prototypes.h $(INCLUDE)/hdd.h \
  $(INCLUDE)/generate.h $(INCLUDE)/config.h

ucvm.o: ucvm.c $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/ucvm.h

clean:
	rm -f clown $(DEVICES) *~ $(libdir)/libdevices.a
