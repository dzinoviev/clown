#include "clown/config.h"
.code $segfault

	mov %ds, $mydata	; BAD:	 we destroy %DS!
	mov %r10, msg
	push %r10
	call puts
	inc %sp
	
	mov %r1, 1		; fix the problem
	retfi

puts:
	push %r1		; save registers
	push %r2		; save registers
	peek %r1, 3 ; stack: %r2[top], %r1[-1], ret.add.[-2], string[-3]
				
write:	
	mov %r2, [%r1]
	cmp %r2, 0		; null character?
	jz endw
				
	out %r2, ?(IOBASE_TTY + 0)
	inc %r1			; go to next character
	jmp write
				
endw:	
	mov %r2, '\n'		; terminating new line
	out %r2, ?(IOBASE_TTY + 0)
				
	pop %r2			; restore the stack
	pop %r1
	retn

.data $mydata
msg:	.string "Division by zero attempted. Trying to fix. If you see this message again, then I guess it didn't work.\n"
	
.data $isr
.global myisr:	.word[16] ; the ISR segment
