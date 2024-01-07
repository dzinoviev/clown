#include "config.h"

;; puts
;; Write a null-terminated string to the terminal output device
.GLOBAL puts:
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
	
;; gets
;; Read a single line from the terminal input device
.GLOBAL gets:
	PUSH %R1		; save registers
	PUSH %R2		; save registers
	PEEK %R1, 3 + 0	; stack: %r2[top], %r1[-1], ret.add.[-2], string[-3]
				
poll:		
	IN %R2, ?(IOBASE_TTY + 2)
	CMP %R2, 1		; is the keyboard ready?
	JNZ poll
				
read:	
	IN %R2, ?(IOBASE_TTY + 1) ; read the next character
	OUT %R2, ?(IOBASE_TTY + 0) ; echo the read character
			
	CMP %R2, '\n'		; are we at the end of the line?
	JZ endr

	MOV [%R1], %R2		; store the character
	INC %R1			; advance the pointer
	JMP poll
				
endr:	
	XOR %R2, %R2
	MOV [%R1], %R2		; store the null terminator
				
	POP %R2			; restore the stack
	POP %R1
	RETN
