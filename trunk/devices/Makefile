INCLUDE = ../include/
CFLAGS = -Wall -ansi  -g -I$(INCLUDE) -O3
DEVICES = tty.o timer.o hdd.o hddlib.o dma.o

all: $(DEVICES)

tty.o:   tty.c   $(INCLUDE)clown_types.h $(INCLUDE)generate.h $(INCLUDE)exceptions.h 
timer.o: timer.c  $(INCLUDE)clown_types.h $(INCLUDE)generate.h $(INCLUDE)exceptions.h
hdd.o:   hdd.c   $(INCLUDE)clown_types.h $(INCLUDE)generate.h $(INCLUDE)exceptions.h $(INCLUDE)hdd.h
hddlib.o:   hddlib.c $(INCLUDE)clown_types.h $(INCLUDE)generate.h $(INCLUDE)exceptions.h $(INCLUDE)hdd.h
dma.o:   dma.c   $(INCLUDE)clown_types.h $(INCLUDE)generate.h $(INCLUDE)exceptions.h $(INCLUDE)hdd.h $(INCLUDE)config.h $(INCLUDE)prototypes.h $(INCLUDE)ucvm.h 

clean:
	rm -f clown $(DEVICES) *~
