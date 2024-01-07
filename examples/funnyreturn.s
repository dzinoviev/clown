;; Simulate a return from a function by pushing a return address to the stack

#include <hardware.h>

	JMP pseudofunction

begin:	MOV %R1, '#'
	OUT %R1, ?(IOBASE_TTY + 0)		; to screen
	STOP

pseudofunction:
	MOV %R0, begin
	PUSH %R0
	RETN
