#ifndef GENERATE_H
#define GENERATE_H 1

/* All sizes are given in WORDS, not in BYTES */
#define CLOWN_FRAME_BITS  10
#define CLOWN_FRAME_MASK  (~(~(Uword)0 << CLOWN_FRAME_BITS))
#define CLOWN_FRAME_SIZE  (1 << CLOWN_FRAME_BITS) /* Frame size, page size */

/* CONFIGURATION OPTION */
/* How much memory do we have? */
#define CLOWN_MEMORY_SIZE (1024 * CLOWN_FRAME_SIZE) 
/* How many TLB entries do we have? Set to 0 for no TLB */
#define CLOWN_TLB_SIZE 16

/* device ID signature */
#define DEVICE_ID 0x7F		

#endif
