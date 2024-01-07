;;; This program fails because of the unhandled DMA interrupt
;;; TODO: fix the interrupt vector
	
#include <hardware.h>

	OUT 4  , ?(IOBASE_HDD + 0) ; interrupt mode
	MOV %R1, oscode		   ; initialize the pointer to  the array
	OUT 0  , ?(IOBASE_DMA + 0) ; reset the controller
	OUT 0  , ?(IOBASE_DMA + 2) ; store track number
	OUT 1  , ?(IOBASE_DMA + 2) ; store sector number
	OUT %R1, ?(IOBASE_DMA + 2) ; store memory location
	OUT 2  , ?(IOBASE_DMA + 0) ; trigger memory-to-disk transfer

	HLT
	STOP
	
oscode:	.STRING "Hello kitty!"
