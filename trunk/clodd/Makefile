INCLUDE = ../include/
DEVICES = ../devices/
CFLAGS = -Wall -ansi  -g -I$(INCLUDE) #-O3
LD = $(CC)

all:  ../bin/clodd

../bin/clodd: clodd.o options.o $(DEVICES)hddlib.o 
	$(LD) $^ -o $@ -lc

clodd.o: clodd.c $(INCLUDE)hdd.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)generate.h clodd.h

options.o: options.c clodd.h

hddlib.o: $(DEVICES)hddlib.c $(INCLUDE)hdd.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)generate.h

clean:
	rm -f ../bin/clodd *.o

