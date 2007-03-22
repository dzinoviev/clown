#include "config.h"
#define DEC 10
	mov %r1, 1234
	stop

printf:	mov  %r10, DEC
	push %r10
	xor  %r0, %r0
	mov %r11, '0'

	cmp %r1, %r0
	jz zero
divide:	
	mov %r2, %r1
	div %r2, %r10
	mul %r2, %r10
	mov %r4, %r1
	sub %r4, %r2
	push %r4
	div %r1, %r10
	cmp %r1, %r0
	jnz divide

do_print:
	pop %r1
	cmp %r1, %r10
	jz done
zero:	add %r1, %r11
	OUT	%R1, (IOBASE_TTY + 0)		; to screen	
	jmp do_print
done:	retn
