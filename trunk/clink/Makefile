include ../Makefile.conf

PFX = rd
OBJS = clink.o options.o 
LEXFLAGS = -i -I -8 -P$(PFX)
YACCFLAGS = -d -p $(PFX)
LDFLAGS = -L$(libdir) -lcode #-lefence

all: clink

clink: $(OBJS) $(libdir)/libcode.a
	$(CC) $^ -o $@ $(LDFLAGS)

clink.o: clink.c clink.h $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

options.o: options.c clink.h $(INCLUDE)/clowndev.h $(INCLUDE)/isa.h \
  $(INCLUDE)/prototypes.h $(INCLUDE)/clown_types.h \
  $(INCLUDE)/exceptions.h $(INCLUDE)/symtab.h

clean:
	rm -f *~ $(OBJS) y.tab.* lex.$(PFX).c clink

install: clink
	mkdir -p $(DESTDIR)/bin
	install -s -m 755 clink $(DESTDIR)/bin
