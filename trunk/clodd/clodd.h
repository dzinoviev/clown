#ifndef CLODD_H
#define CLODD_H

#include "hdd.h"

extern int sector;
extern int track;
extern int size;
extern int unpack;
extern enum Clodd_Dir { UNDEF, TODISK, FROMDISK } direction;
extern char *file;
void read_options (int argc, char *argv[]);
#endif
