#include "clown/config.h"
.CODE $segfault

	MOV %DS, $mydata	; BAD:	 we destroy %DS!
	MOV %R10, msg
	PUSH %R10
	CALL puts
	INC %SP
	
	MOV %R1, 1		; fix the problem
	RETFI

puts:
	PUSH %R1		; save registers
	PUSH %R2		; save registers
	PEEK %R1, 3 ; stack: %r2[top], %r1[-1], ret.add.[-2], string[-3]
				
write:	
	MOV %R2, [%R1]
	CMP %R2, 0		; null character?
	JZ endw
				
	OUT %R2, ?(IOBASE_TTY + 0)
	INC %R1			; go to next character
	JMP write
				
endw:	
	MOV %R2, '\n'		; terminating new line
	OUT %R2, ?(IOBASE_TTY + 0)
				
	POP %R2			; restore the stack
	POP %R1
	RETN

.DATA $mydata
msg: .STRING "Division by zero attempted. Trying to fix it.\nIf you see this message again, then I failed.\n"
	
.DATA $isr
.GLOBAL myisr:	.WORD[16] ; the ISR segment
