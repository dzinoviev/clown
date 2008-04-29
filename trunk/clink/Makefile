INCLUDE = ../include
CFLAGS = -Wall -g -I$(INCLUDE) -O3 -ansi #-pedantic
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

clink.o: clink.c clink.h $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

options.o: options.c clink.h $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

clean:
	rm -f *~ $(OBJS) y.tab.* lex.$(PFX).c

cleanall:
	rm -f *~ $(OBJS) y.tab.* lex.$(PFX).c ../bin/clink 
