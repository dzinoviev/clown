#include "config.h"

iv:	jmp setup
	.word[15]
	
setup:	
	mov     %r3, timerInterrupt
        or	%r3, 1
	mov	%r2, iv
	add	%r2, 8
        mov     [%r2], %r3
	
        mov     %r1, 1         ;   reset counter
        out     %r1, (IOBASE_TIMER + 0)
        mov     %r1, -5000000      ;   set the counter
        out     %r1, (IOBASE_TIMER + 0)
        mov     %r1, 5000000      ;   set the counter
        out     %r1, (IOBASE_TIMER + 0)

again:  hlt                    ;     wait for an interupt
	jmp	again


;;;  Handle timer interrupt              
;;;  For the moment it will just return from an near interrupt

.align8 timerInterrupt:
	mov %r1, '*'
	OUT	%R1, (IOBASE_TTY + 0)		; to screen	
	mov %r1, '\n'
	OUT	%R1, (IOBASE_TTY + 0)		; to screen	
        retni
