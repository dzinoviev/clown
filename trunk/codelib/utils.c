#include <stdio.h>
#include <unistd.h>
#include "clowndev.h"

jmp_buf failure;

void secure_write (int file, void *addr, int size)
{
  if (size != write (file, addr, size)) {
    perror ("Output file");
    longjmp (failure, 1);
  }    
}

void secure_string (int file, char *string)
{
  size_t len = strlen (string);
  secure_write (file, string, len);
}

void *safe_malloc (size_t size)
{
  void *ptr = malloc (size);
  if (!ptr) {
    perror ("malloc");
    longjmp (failure, 1);
  }
  return ptr;
}
      
void *safe_realloc (void *ptr, size_t size)
{
  if (!(ptr = realloc (ptr, size))) {
    perror ("realloc");
    longjmp (failure, 1);
  }
  return ptr;
}
