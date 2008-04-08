#ifndef CLINK_H
#define CLINK_H 1

#ifndef CLOWNDEV_H
#include "clowndev.h"
#endif

int get_options (int argc, char *argv[], char **object,
		 char ***source, int *nsources, int *ecode);

extern int listing;
extern int silent;
extern Clof_Type module_type;
#endif
