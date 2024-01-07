;;; Run as `clown -q timerinter.cle` to suppress interrupt messages

#include <hardware.h>

iv:	JMP setup		; Interrupt vector
	.WORD[15]
	
setup:	
	MOV     %r3, timerInterrupt
        OR	%r3, 1
	MOV	%r2, iv
	ADD	%R2, 8
        MOV     [%R2], %R3
	
        MOV     %R1, 1         ;   reset counter
        OUT     %R1, ?(IOBASE_TIMER + 0)
        MOV     %R1, -100000000      ;   set the counter
        OUT     %R1, ?(IOBASE_TIMER + 0)
        MOV     %R1, 100000000      ;   set the counter
        OUT     %R1, ?(IOBASE_TIMER + 0)

again:  HLT                    ;     wait for an interupt
	JMP	again


;;;  Handle timer interrupt              
;;;  For the moment it will just return from an near interrupt

.ALIGN8 timerInterrupt:
	MOV %R1, '*'
	OUT	%R1, ?(IOBASE_TTY + 0)		; to screen	
	MOV %R1, '\n'
	OUT	%R1, ?(IOBASE_TTY + 0)		; to screen	
        RETNI
