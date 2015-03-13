;; Simulate a return from a function by pushing a return address to the stack

#include <hardware.h>

	jmp pseudofunction

begin:	mov %r1, '$'
	out	%r1, ?(IOBASE_TTY + 0)		; to screen
	stop

pseudofunction:
	mov %r0, begin
	push %r0
	retn
