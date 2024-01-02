#ifndef CLODD_H
#define CLODD_H

#include "clowndev.h"
#include "hdd.h"

extern unsigned sector;
extern unsigned track;
extern unsigned size;
extern int unpack;
extern enum Clodd_Dir { UNDEF, TODISK, FROMDISK } direction;
extern char *file;
void read_options (int argc, char *argv[]);
#endif
