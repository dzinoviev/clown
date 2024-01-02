#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include "clowndev.h"

jmp_buf failure;

static void safe_write (int file, void *addr, int size)
{
  if (size != write (file, addr, size)) {
    perror ("Output file");
    longjmp (failure, 1);
  }    
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

#define BASE64_SYM 6
#define BASE64_ZIPFACTOR (int)((sizeof(Dword) * CHAR_BIT) / BASE64_SYM)
#define BASE64_MASK(x) ((1<<(x))-1)
#define BASE64_BUFSIZE 16
#define BASE64_PHASES (BASE64_BUFSIZE / BASE64_ZIPFACTOR)
static unsigned char buffer_base64[BASE64_BUFSIZE] = {0};
static int base64_phase = 0;
static const unsigned char base64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int unbase64[] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
    };


int base64_decode (char *orig, Dword *decoded)
{
  int i = 0;
  int unbase64_phase = 0;
  Dword *start = decoded;
  uint32_t my_word = 0;

  for (char *cur = orig; *cur; cur++) {
    int shift;
    int32_t d = unbase64[(int)*cur];
    if (d == -1) {		/* bad character */
      return -1;
    }
    shift = i * BASE64_SYM - 2 * unbase64_phase;
    if (shift >= 0)
      my_word |= (d << shift);
    else
      my_word |= (d >> -shift);
    if (i < BASE64_ZIPFACTOR) {
      i++;
    } else {
      *decoded++ = ntohl (my_word);
      my_word = 0;
      unbase64_phase = (unbase64_phase + 1) % BASE64_PHASES;
      if (unbase64_phase > 0)
	cur--;
      i = 0;
   }
  }

  if (i > 0) 
    *decoded++ = ntohl (my_word);
  return decoded - start;
}

void safe_base64 (int file, Dword word)
{
  int i;
  uint32_t my_word = htonl (word);

  buffer_base64[base64_phase * BASE64_ZIPFACTOR] 
    |= (my_word & BASE64_MASK(BASE64_SYM - 2 * base64_phase)) << (2 * base64_phase);

  for (i = 1; i <= BASE64_ZIPFACTOR; i++) {
    uint32_t shifted;
    int shift = BASE64_SYM * i - 2 * base64_phase;
    if (shift >= 0)
      shifted = my_word >> shift;
    else
      shifted = my_word << -shift;
      
    buffer_base64[i + base64_phase * BASE64_ZIPFACTOR] = shifted & BASE64_MASK (BASE64_SYM);
  }

  base64_phase = (base64_phase + 1) % BASE64_PHASES;
  
  if (base64_phase == 0) {
    for (i = 0; i < BASE64_BUFSIZE; i++)
      buffer_base64[i] = base64[buffer_base64[i]];
    safe_write (file, buffer_base64, BASE64_BUFSIZE);
    memset (buffer_base64, 0, sizeof (buffer_base64));
  }
}

static void finalize_base64 (int file)
{
  if (base64_phase == 0)
    return;
  for (int i = 0; i < base64_phase * BASE64_ZIPFACTOR + 1; i++)
    buffer_base64[i] = base64[buffer_base64[i]];
  safe_write (file, buffer_base64, base64_phase * BASE64_ZIPFACTOR + 1);
  base64_phase = 0;
  memset (buffer_base64, 0, sizeof (buffer_base64));
}

void safe_string (int file, char *string)
{
  size_t len = strlen (string);
  finalize_base64 (file);
  safe_write (file, string, len);
}

