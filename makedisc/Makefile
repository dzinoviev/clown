INCLUDE = ../include/
CFLAGS = -Wall -ansi  -g -I$(INCLUDE) #-O3
LD = $(CC)

all:  ../bin/makedisc

../bin/makedisc: makedisc.o
	$(LD) $^ -o $@ -lc

makedisc.o: makedisc.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h $(INCLUDE)hdd.h

clean:
	rm -f ../bin/makedisc *.o

disc:
	../bin/makedisc 64 32 30000 260000 83000