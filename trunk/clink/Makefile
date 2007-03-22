INCLUDE = ../include/
CFLAGS = -Wall -g -ansi -I$(INCLUDE) #-O3
CAS = ../cas
OBJS = clink.o options.o $(CAS)/common.o
LD = $(CC)
LEXFLAGS = -CF
YACCFLAGS = -d
#LDFLAGS = -lfl

all: ../bin/clink

../bin/clink: $(OBJS)
	$(LD) $^ -o $@ -lc

clink.o: clink.c clink.h $(INCLUDE)clowndev.h
options.o: options.c  clink.h

clean:
	rm -f $(OBJS) y.tab.* lex.yy.c clink-yacc.tab.* clink-yacc.output ../bin/clink 
