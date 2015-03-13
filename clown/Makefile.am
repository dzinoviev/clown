## Process this file with automake to produce Makefile.in

bin_PROGRAMS = clown
BUILT_SOURCES = cmd-yacc.h
AM_LFLAGS = -i -I --prefix=cmd --outfile=lex.yy.c
AM_YFLAGS = -d --name-prefix=cmd
clown_LDADD = ../codelib/libclowncode.la ../devlib/libclowndevices.la
clown_SOURCES = clown.c decode.c paging.c segmentation.c general.c tlb.c options.c readln.c  load.c cmd-yacc.y cmd-lex.l

