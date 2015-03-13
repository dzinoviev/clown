## Process this file with automake to produce Makefile.in

AM_LFLAGS = -CF -8
AM_YFLAGS = -d
AM_CFLAGS += -D'INCLUDEHOME="$(pkgincludedir)"'
BUILT_SOURCES = cas-yacc.h
bin_PROGRAMS = cas
cas_LDADD = ../codelib/libclowncode.la
cas_SOURCES = cas.c options.c parse.c cas-yacc.y cas-lex.l cas.h

