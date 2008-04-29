#include "config.h"
#define PUT(x) out x, ?(IOBASE_TTY + 0)
;; CALLER
;; It is the responsibility of the caller to push the parameters onto the stack
;; and to clean up the stack after the call, if necessary.
;; We will be pushing parameters in the reverse order.
;; If the function takes no parameters, but returns values, it is the 
;; responsibility of the caller to reserve at least one slot in the stack.
	push	'5'	; push the third parameter
	push	'0'	; push the second parameter
	push	'6'	; push the first parameter
	call	func	; call the function
	pop	%r1	; save the return value
	sub	%sp, 2	; restore the stack, if necessary (still has 2 words)
;	...	
	PUT (%r1)	; use the return value
	stop

;; FUNCTION
;; It is the responsibility of the function to save the values of ALL registers
;; it plans to use, and to restore them on exit.
func:	
	push	%r1	; save registers
	push	%r2	; save registers
	push	%r3	; save registers
	
	peek	%r1, 4+0	; access the 1st argument (4 = 3 saves + 1 PC)
	peek	%r2, 4+1	; access the 2nd argument
	peek	%r3, 4+2	; access the 3rd argument

			; use arguments
	PUT (%r1)
	PUT (%r2)
	PUT (%r3)

	mov	%r1, '\n'	
	poke	%r1, 4+0	; store the return value in the stack

	;; pop them in the reverse order!
	pop	%r3	; restore saved registers
	pop	%r2	; restore saved registers
	pop	%r1	; restore saved registers
	
	retn
