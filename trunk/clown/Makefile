INCLUDE = ../include
CODELIB = ../codelib/
DEVICELIB = ../devlib/
CFLAGS = -Wall -g -I$(INCLUDE) -O3 -ansi# -pedantic
# You must link this library if TTY_PTHREAD is defined
LIBPTHREAD = -pthread
LDFLAGS = -lreadline -lcurses -L$(CODELIB) -lcode -L$(DEVICELIB) -ldevices $(LIBPTHREAD) #-lefence
CMD = cmd
LEXFLAGS = -i -I -P$(CMD)
YACCFLAGS = -d -p $(CMD)
OBJ = clown.o decode.o paging.o segmentation.o general.o ioint.o tlb.o options.o cache.o  readln.o commandline.tab.o lex.$(CMD).o load.o ui.o

all: ../bin/clown

../bin/clown: $(OBJ) $(CODELIB)/libcode.a $(DEVICELIB)/libdevices.a
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clown.o:clown.c $(INCLUDE)/readln.h $(INCLUDE)/clown/config.h \
  $(INCLUDE)/registers.h $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h \
  $(INCLUDE)/clown_types.h $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h \
  $(INCLUDE)/clowndev.h $(INCLUDE)/symtab.h

ui.o:  ui.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

decode.o: decode.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h

general.o: general.c $(INCLUDE)/options.h $(INCLUDE)/registers.h \
  $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h

paging.o: paging.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h

segmentation.o: segmentation.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/exceptions.h

ioint.o: ioint.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

options.o: options.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h version.h \
  $(INCLUDE)/clowndev.h $(INCLUDE)/symtab.h

tlb.o: tlb.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

cache.o: cache.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

load.o: load.c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h $(INCLUDE)/clowndev.h \
  $(INCLUDE)/symtab.h

readln.o: readln.c $(INCLUDE)/readln.h

lex.$(CMD).o: lex.$(CMD).c $(INCLUDE)/registers.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h commandline.tab.h

commandline.tab.o: commandline.tab.c $(INCLUDE)/registers.h \
  $(INCLUDE)/isa.h $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/generate.h

commandline.tab.c: commandline.y 
	bison $(YACCFLAGS) commandline.y

lex.$(CMD).c: commandline.l
	flex $(LEXFLAGS) $<

clean:
	rm -f $(OBJ)  commandline.tab.c commandline.tab.h lex.$(CMD).c *~

cleanall:
	rm -f ../bin/clown $(OBJ) commandline.tab.c commandline.tab.h lex.$(CMD).c *~

