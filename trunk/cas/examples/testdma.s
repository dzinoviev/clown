#include "config.h"

	out 4  , IOBASE_HDD + 0 ; interrupt mode
	mov %r1, oscode		; initialize the pointer to  the array
	out 0  , IOBASE_DMA + 0 ; reset the controller
	out 0  , IOBASE_DMA + 2 ; store track number
	out 1  , IOBASE_DMA + 2 ; store sector number
	out %r1, IOBASE_DMA + 2 ; store memory location
	out 2  , IOBASE_DMA + 0 ; trigger memory-to-disk transfer

	hlt
	stop

.align8 oscode:	.string "Hello kitty!"
