#include "config.h"

;; puts
;; Write a null-terminated string to the terminal output device
.global puts:
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
	
;; gets
;; Read a single line from the terminal input device
.global gets:
	push %r1		; save registers
	push %r2		; save registers
	peek %r1, 3 + 0	; stack: %r2[top], %r1[-1], ret.add.[-2], string[-3]
				
poll:		
	in %r2, ?(IOBASE_TTY + 2)
	cmp %r2, 1		; is the keyboard ready?
	jnz poll
				
read:	
	in %r2, ?(IOBASE_TTY + 1) ; read the next character
	out %r2, ?(IOBASE_TTY + 0) ; echo the read character
			
	cmp %r2, '\n'		; are we at the end of the line?
	jz endr

	mov [%r1], %r2		; store the character
	inc %r1			; advance the pointer
	jmp poll
				
endr:	
	mov %r2, 0
	mov [%r1], %r2		; store the null terminator
				
	pop %r2			; restore the stack
	pop %r1
	retn
