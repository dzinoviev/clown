INCLUDE = ../include/
CODELIB = ../codelib/
DEVICELIB = ../devlib/
CFLAGS = -Wall -ansi -g -I$(INCLUDE) -O3 
LDFLAGS = -lreadline -lcurses -L$(CODELIB) -lcode -L$(DEVICELIB) -ldevices #-lefence
CMD = cmd
LEXFLAGS = -i -I -P$(CMD)
YACCFLAGS = -d -p $(CMD)

all: ../bin/clown

../bin/clown: clown.o decode.o paging.o segmentation.o general.o ioint.o tlb.o options.o cache.o  readln.o commandline.tab.o lex.$(CMD).o load.o $(CODELIB)/libcode.a $(DEVICELIB)/libdevices.a
	$(CC) $^ -o $@ $(LDFLAGS)

clown.o: clown.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/config.h $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

decode.o: decode.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/prototypes.h

general.o: general.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

paging.o: paging.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

segmentation.o: segmentation.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

ioint.o: ioint.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

options.o: options.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h version.h

tlb.o: tlb.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

cache.o: cache.c $(INCLUDE)/isa.h $(INCLUDE)/exceptions.h $(INCLUDE)/registers.h  $(INCLUDE)/generate.h $(INCLUDE)/clown_types.h $(INCLUDE)/prototypes.h

load.o: load.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/clowndev.h \
  $(INCLUDE)/symtab.h

readln.o: readln.c $(INCLUDE)/readln.h

lex.$(CMD).o: lex.$(CMD).c commandline.tab.h 

lex.$(CMD).c: commandline.l
	flex $(LEXFLAGS) $<

commandline.tab.c: commandline.y 
	bison $(YACCFLAGS) commandline.y

commandline.tab.o: commandline.tab.c $(INCLUDE)/registers.h \
  $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

clean:
	rm -f *.o commandline.tab.c commandline.tab.h lex.$(CMD).c *~

cleanall:
	rm -f ../bin/clown *.o commandline.tab.c commandline.tab.h lex.$(CMD).c *~

