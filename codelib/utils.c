#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "clowndev.h"

jmp_buf failure;

static void safe_write(int file, void *addr, int size)
{
  if (size != write(file, addr, size)) {
    perror("Output file");
    longjmp(failure, 1);
  }    
}

void *safe_malloc(size_t size)
{
  void *ptr = malloc(size);
  if (!ptr) {
    perror("malloc");
    longjmp(failure, 1);
  }
  return ptr;
}
      
void *safe_realloc(void *ptr, size_t size)
{
  if (!(ptr = realloc(ptr, size))) {
    perror("realloc");
    longjmp(failure, 1);
  }
  return ptr;
}

int base64_decode(char *orig, Dword *decoded)
{
  int l = strlen(orig) / 8;
  for(int i = 0; i < l; i++) 
    sscanf(&orig[8*i], "%08X", &decoded[i]);
  return l;
}

void safe_base64(int file, Dword word)
{
  char b[16];  // Enough for a Dword
  sprintf(b, "%08X", word);
  safe_write(file, b, 8);
}

void safe_string(int file, char *string)
{
  safe_write(file, string, strlen(string));
}
