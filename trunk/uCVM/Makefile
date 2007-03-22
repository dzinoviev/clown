INCLUDE = ../include/
CFLAGS = -Wall -ansi -g -I$(INCLUDE) -O3 
LDFLAGS = #-Xlinker -E -ldl

all: ucvm.o

ucvm.o: ucvm.c $(INCLUDE)ucvm.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h 

clean:
	rm -f ucvm.o
