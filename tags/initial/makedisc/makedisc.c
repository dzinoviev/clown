#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "hdd.h"

#define VERSION "CLOWN disc formatting utility version 1.00 ["__DATE__"]"

static void show_version (void)
{
    fputs (VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs ("Copyright 2004 D. Zinoviev\n"
	   "This program is free software; you may redistribute it under the terms of\n"
	   "the GNU General Public License.  This program has absolutely no warranty.\n\n", 
	   stderr);
}


int main (int argn, char *argv[])
{
    struct Clown_Hdd params;
    int image, i;
    Uword delta_phi, n_words;
    Uword zero = 0, sep;

    show_version ();
    show_copy ();

    if (argn != 6) {
	fprintf (stderr, 
		 "Usage: %s n_tracks n_sectors t2t_seek max_seek rot_speed\n", 
		 argv[0]);
	fputs ("\tt2t_seek, max_seek, and rot_speed are in CPU cycles\n", stderr);
	fprintf (stderr, "\tExample: %s 64 32 30000 260000 100000\n", argv[0]);
	fputs ("New disk image will be written into file \""
	       DISC_IMAGE"\".\n", stderr);
	return EXIT_SUCCESS;
    }

    image = open (DISC_IMAGE, O_RDONLY);
    if (image != -1) {
	char s[8];
	close (image);
	fputs ("File \""DISC_IMAGE"\" already exists.\n"
	       "Do you want to overwrite it? [N/y] ", stderr);
	fgets (s, 8, stdin);
	switch (s[0]) {
	case 'y':
	case 'Y':
	    break;
	default:
	    return EXIT_SUCCESS;
	}
    }
    
    params.n_tracks  = atoi (argv[1]);
    params.n_sectors = atoi (argv[2]);
    params.t2t_seek  = atoi (argv[3]);
    params.max_seek  = atoi (argv[4]);
    params.rot_speed = atoi (argv[5]);

    if (   params.n_tracks  <= 2
	|| params.n_sectors <= 0 
	|| params.t2t_seek  <= 0
	|| params.max_seek  <= 0 
	|| params.rot_speed <= 0) {
	fputs ("All parameters must be positive integer numbers.\n", stderr);
	fputs ("There must be at least 3 tracks.\n", stderr);
	return EXIT_FAILURE;
    }

    n_words = params.n_sectors * (DISC_WORDS_PER_SECTOR + DISC_WORDS_PER_GAP);
    delta_phi = params.rot_speed / n_words;
    if (delta_phi == 0)
	delta_phi = 1;		/* the disc must rotate, anyway! */
    params.rot_speed = delta_phi * n_words;

    fprintf (stderr, "Adjusted rotation speed: %lu cycles/rev.\n", 
	     params.rot_speed);
    fprintf (stderr, "Sector size:      %7u W\n", DISC_WORDS_PER_SECTOR);
    fprintf (stderr, "Sector separator: %7u W\n", DISC_WORDS_PER_GAP);

    image = open (DISC_IMAGE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (sizeof (struct Clown_Hdd) 
	!= write (image, &params, sizeof (struct Clown_Hdd))) {
	perror (DISC_IMAGE);
	close (image);
	return EXIT_FAILURE;	
    }

    memcpy (&sep, "########", sizeof (Uword));
    for (i = 0; i < params.n_tracks * params.n_sectors; i++) {
	int j;
	for (j = 0; j < DISC_WORDS_PER_SECTOR; j++)
	    if (sizeof (Dword) != write (image, &zero, sizeof (Dword))) {
		perror (DISC_IMAGE);
		close (image);
		return EXIT_FAILURE;	
	    }	
	for (j = 0; j < DISC_WORDS_PER_GAP; j++)
	    if (sizeof (Dword) != write (image, &sep, sizeof (Dword))) {
		perror (DISC_IMAGE);
		close (image);
		return EXIT_FAILURE;	
	    }	
    }

    close (image);
    n_words *= params.n_tracks;
    fprintf (stderr, "Total disc volume: %6lukW.\n", n_words / 1024);
    return EXIT_SUCCESS;
}
