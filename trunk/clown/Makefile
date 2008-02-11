INCLUDE = ../include/
CFLAGS = -Wall -ansi -g -I$(INCLUDE) -O3 
LDFLAGS = -lfl -lreadline -lcurses #-lefence
LD = $(CC)
LEXFLAGS = -i -I
YACCFLAGS = -d

DEVICES = ../devices/timer.o ../devices/tty.o ../devices/dma.o ../uCVM/ucvm.o ../devices/hdd.o ../devices/hddlib.o
all: ../bin/clown

../bin/clown: clown.o decode.o paging.o segmentation.o general.o ioint.o tlb.o options.o cache.o debug.o readln.o $(DEVICES) commandline.tab.o lex.yy.o 
	$(LD) $^ -o $@ $(LDFLAGS)

clown.o: clown.c $(INCLUDE)registers.h $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)config.h $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h
decode.o: decode.c $(INCLUDE)registers.h $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)generate.h $(INCLUDE)prototypes.h
general.o: general.c $(INCLUDE)registers.h $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h
paging.o: paging.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h
segmentation.o: segmentation.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h
ioint.o: ioint.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h

options.o: options.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h

tlb.o: tlb.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h

cache.o: cache.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h

debug.o: debug.c $(INCLUDE)isa.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h  $(INCLUDE)generate.h $(INCLUDE)clown_types.h $(INCLUDE)prototypes.h

readln.o: readln.c $(INCLUDE)readln.h

lex.yy.o: lex.yy.c commandline.tab.h 

lex.yy.c: commandline.l
	flex $(LEXFLAGS) $<

commandline.tab.c: commandline.y 
	bison $(YACCFLAGS) commandline.y

commandline.tab.o: commandline.tab.c

clean:
	rm -f ../bin/clown *.o commandline.tab.c lex.yy.c *~
