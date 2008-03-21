#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <unistd.h>
#include "registers.h"

typedef struct {
  Dword offset;
  char *file;
  int line;
} ModInfo;

typedef struct {
  Dword offset;
  Dword size;
  Dword lines;
  ModInfo *info;
} Module;

typedef struct {
  int fid;
  char *fname;
} ModSource;

static ModSource *modsrcs = NULL;
static int n_modsrcs = 0;
static Module *modules = NULL;
static int n_modules = 0;

static ModInfo *lookup_debug_info (Dword address)
{
  int j;
  /* Search bottom up */
  for (j = n_modules - 1; j >= 0; j--) {
    int i;
    Module m;
    if (address < modules[j].offset || address >= modules[j].offset + modules[j].size)
      continue;
    m = modules[j];
    address -= m.offset;
    for (i = m.lines - 1; i >= 0; i--) {
      if (address >= m.info[i].offset)
	return &(m.info[i]);
    }
    return NULL;		/* is this possible? */
  }
  return NULL;
}

void print_debug_info (Dword address)
{ 
    ModInfo *mi = lookup_debug_info (address);
    if (mi) {
	fprintf (stderr, " [%s:%d]", mi->file, mi->line);
    }
}

static int parse_debug_info (FILE * file, Dword offset, Dword size, time_t dfile_mod_time)
{
  int nfiles, i, n_lines = 0;
  int cmd_segment, cmd_offset, fileno, lineno;
  ModInfo *info = NULL;

  /* read the number of source files in the project */
  if (1 != fscanf (file, "%d", &nfiles))
    return 0;
  
  info = malloc (sizeof (ModInfo) * size);
  if (!info) {
    perror ("malloc");
    return 0;
  }
      
  /* read the names and the id's of the source files */
  for (i = 0; i < nfiles; i++) {
    int fno;
    char fname[PATH_MAX];
    struct stat buf;

    if (2 != fscanf (file, "%d %s", &fno, fname))
      return 0;

    /* Is the source file newer than the debug file? */
    if (-1 == stat (fname, &buf)) {
      perror (fname);
      return 0;
    }
    
    if (buf.st_mtime >= dfile_mod_time) {
	if (!silent)
	    fprintf (stderr, "--> Source file %s is newer than the debug file.\n", fname);
      return 0;
    }

    /* Allocate extra space for source file name(s) */
    modsrcs = realloc (modsrcs, sizeof (ModSource) * (n_modsrcs + 1));
    modsrcs[n_modsrcs].fid = fno;
    modsrcs[n_modsrcs].fname = malloc (strlen (fname) + 1);
    if (!modsrcs[n_modsrcs].fname) {
      perror ("malloc");
      return 0;
    }
    strcpy (modsrcs[n_modsrcs].fname, fname);    
    n_modsrcs++;

    /* Load the line numbers, etc. */
    while (4 == fscanf (file, "%d %d %d %d", &cmd_segment, &cmd_offset, &fileno, &lineno)) {
      int j;
      if (cmd_segment != -1) {
	  if (!silent)
	      fprintf (stderr, "--> Source file %s contains non-default segments.\n", fname);	
	return n_lines;
      }
      info[n_lines].offset = cmd_offset;
      info[n_lines].line = lineno;

      for (j = n_modsrcs - 1; n_modsrcs >=0; n_modsrcs--) {
	if (fileno == modsrcs[j].fid) {
	  info[n_lines].file = modsrcs[j].fname;
	  break;
	}
      }
      if (j < 0) {
	  if (!silent)
	      fprintf (stderr, "--> Malformed debug file.\n");	
	return n_lines;	
      }

      n_lines++;
    }
  }

  /* Cannot have more rows then lines! */
  info = realloc (info, sizeof (ModInfo) * n_lines);
  if (!info) {
    perror ("malloc");
    return 0;
  }

  /* Allocate extra space for this module */
  modules = realloc (modules, sizeof (Module) * (n_modules + 1));
  if (!modules) {
    perror ("malloc");
    return 0;
  }
  modules[n_modules].offset = offset;
  modules[n_modules].size = size;
  modules[n_modules].lines = n_lines;
  modules[n_modules].info = info;
  n_modules++;

  return n_lines;
}

void load_debug_info (char *fname, Dword offset, Dword size)
{
  char *dfile;
  FILE *debug_info;

  dfile = malloc (strlen (fname) + 5);
  if (!dfile) {
    perror ("malloc");
  } else {
    struct stat buf;
    strcpy (dfile, fname);
    strcat (dfile, ".dbg");
    
    if (-1 == stat (dfile, &buf) 
	|| !((buf.st_mode & S_IRUSR) || (buf.st_mode & S_IRGRP) || (buf.st_mode & S_IROTH))) {
	if (!silent)
	    fprintf (stderr, "--> File %s does not exist or is not readable.\n"
		     "--> Debug info is not available.\n\n", dfile);
      free (dfile);
      return;
    } 

    debug_info = fopen (dfile, "r");
    if (!debug_info) {
      perror (dfile);
    } else {
      int n_lines;
      n_lines = parse_debug_info (debug_info, offset, size, buf.st_mtime);
      if (!silent) {
	  if (!n_lines) {
	      fputs ("--> Debug info is not available.\n\n", stderr);
	  } else {
	      fprintf (stderr, "--> %d lines of debug info loaded from %s\n\n", n_lines, dfile);
	  }
      }
     fclose (debug_info);
    }
    
    free (dfile);
  }
}
