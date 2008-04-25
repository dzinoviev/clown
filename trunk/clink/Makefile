INCLUDE = ../include/
CFLAGS = -Wall -g -ansi -I$(INCLUDE) #-O3
CAS = ../cas/
PFX = rd
OBJS = clink.o options.o 
LEXFLAGS = -i -I -8 -P$(PFX)
YACCFLAGS = -d -p $(PFX)
CODELIB = ../codelib
LDFLAGS = -L$(CODELIB) -lcode #-lefence

all: ../bin/clink

../bin/clink: $(OBJS) $(CODELIB)/libcode.a
	$(CC) $^ -o $@ $(LDFLAGS)

clink.o: clink.c clink.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h

options.o: options.c clink.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h  $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h

lex.$(PFX).o:  lex.$(PFX).c $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)symtab.h readclof.tab.h

lex.$(PFX).c: readclof.l
	flex $(LEXFLAGS) readclof.l

readclof.tab.c readclof.tab.h: readclof.y 
	bison $(YACCFLAGS) readclof.y

readclof.tab.o: readclof.tab.c $(INCLUDE)clowndev.h $(INCLUDE)isa.h  $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h clink.h

clean:
	rm -f *~ $(OBJS) y.tab.* lex.$(PFX).c readclof.tab.* readclof.output

cleanall:
	rm -f *~ $(OBJS) y.tab.* lex.$(PFX).c readclof.tab.* readclof.output ../bin/clink 
