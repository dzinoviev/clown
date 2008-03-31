INCLUDE = ../include/
CFLAGS = -Wall -g -ansi -I$(INCLUDE) #-O3
CAS = ../cas
OBJS = clink.o options.o $(CAS)/common.o $(CAS)/expressions.o clink-yacc.tab.o lex.yy.o misc.o
LD = $(CC)
LEXFLAGS = -i -I -8
YACCFLAGS = -d
LDFLAGS = -lfl

all: ../bin/clink

../bin/clink: $(OBJS)
	$(LD) $^ -o $@ -lc $(LDFLAGS)

clink.o: clink.c clink.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h

options.o: options.c clink.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h  $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h

misc.o: misc.c $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h clink.h

$(CAS)/common.o: $(CAS)/common.c $(INCLUDE)registers.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)generate.h $(CAS)/cas.h $(INCLUDE)clowndev.h $(INCLUDE)symtab.h

$(CAS)/expressions.o: $(CAS)/expressions.c $(INCLUDE)registers.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)generate.h $(CAS)/cas.h  $(INCLUDE)clowndev.h $(INCLUDE)symtab.h $(CAS)/cas-yacc.tab.h

lex.yy.o:  lex.yy.c $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)symtab.h clink-yacc.tab.h clink.h

lex.yy.c: clink-lex.l
	flex $(LEXFLAGS) clink-lex.l

clink-yacc.tab.c clink-yacc.tab.h: clink-yacc.y 
	bison $(YACCFLAGS) clink-yacc.y

clink-yacc.tab.o: clink-yacc.tab.c $(INCLUDE)clowndev.h $(INCLUDE)isa.h  $(INCLUDE)prototypes.h $(INCLUDE)clown_types.h  $(INCLUDE)exceptions.h $(INCLUDE)symtab.h clink.h

clean:
	rm -f *~ $(OBJS) y.tab.* lex.yy.c clink-yacc.tab.* clink-yacc.output ../bin/clink 
