INCLUDE = ../include/
CFLAGS = -Wall -g -ansi -I$(INCLUDE) -O3
OBJS = cas.o options.o parse.o cas-yacc.tab.o lex.yy.o
LD = $(CC)
LEXFLAGS = -CF -8
YACCFLAGS = -d
CODELIB = ../codelib
LDFLAGS = -L$(CODELIB) -lcode #-lefence

all: ../bin/cas

../bin/cas: $(OBJS) $(CODELIB)/libcode.a
	$(LD) $^ -o $@ $(LDFLAGS)

cas.o: cas.c cas.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h

options.o: options.c cas.h $(INCLUDE)isa.h $(INCLUDE)clowndev.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h

parse.o: parse.c cas.h $(INCLUDE)isa.h $(INCLUDE)clowndev.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)generate.h

lex.yy.o: lex.yy.c cas.h cas-yacc.tab.h cas.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h $(INCLUDE)generate.h

lex.yy.c: cas-lex.l
	flex $(LEXFLAGS) cas-lex.l

cas-yacc.tab.c cas-yacc.tab.h: cas-yacc.y cas.h $(INCLUDE)clowndev.h $(INCLUDE)isa.h $(INCLUDE)clown_types.h $(INCLUDE)exceptions.h $(INCLUDE)registers.h $(INCLUDE)generate.h
	bison $(YACCFLAGS) cas-yacc.y

clean:
	rm -f $(OBJS) y.tab.* lex.yy.c cas-yacc.tab.* cas-yacc.output *~

cleanall:
	rm -f $(OBJS) y.tab.* lex.yy.c cas-yacc.tab.* cas-yacc.output ../bin/cas *~
