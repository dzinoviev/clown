#ifndef CLINK_H
#define CLINK_H 1

int get_options (int argc, char *argv[], char **object,
		 char ***source, int *nsources, int *ecode);

extern int listing;
extern int silent;
extern int no_unresolved_segments;
#endif
