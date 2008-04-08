OBJS = expressions.o writecode.o utils.o readclof.tab.o lex.$(PFX).o
INCLUDE = ../include/
CFLAGS = -I$(INCLUDE) -Wall -g #-O3
PFX = rd
LEXFLAGS = -i -I -8 -P$(PFX)
YACCFLAGS = -d -p $(PFX)

all:	libcode.a

libcode.a: $(OBJS)
	ar -rcs libcode.a $(OBJS)

expressions.o: expressions.c $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

writecode.o: writecode.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/clowndev.h \
  $(INCLUDE)/symtab.h

utils.o: utils.c $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

lex.$(PFX).o:  lex.$(PFX).c $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h readclof.tab.h

lex.$(PFX).c: readclof.l
	flex $(LEXFLAGS) readclof.l

readclof.tab.c readclof.tab.h: readclof.y 
	bison $(YACCFLAGS) readclof.y

readclof.tab.o: readclof.tab.c $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h 

clean:
	rm -f $(OBJS) *~ readclof.tab.c readclof.tab.h lex.rd.c 

cleanall:
	rm -f $(OBJS) *~ readclof.tab.c readclof.tab.h lex.rd.c libcode.a