INCLUDE = ../include/
CFLAGS = -Wall -ansi  -g -I$(INCLUDE) #-O3

all:  ../bin/makedisc

../bin/makedisc: makedisc.o
	$(CC) $^ -o $@ 

makedisc.o: makedisc.c $(INCLUDE)/hdd.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

clean:
	rm -f *.o *~

cleanall:
	rm -f ../bin/makedisc *.o

disc:
	../bin/makedisc 64 32 30000 260000 83000