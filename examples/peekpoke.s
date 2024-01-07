#include <hardware.h>
#define PUT(x) OUT x, ?(IOBASE_TTY + 0)

;; CALLER
;; It is the responsibility of the caller to push the parameters onto the stack
;; and to clean up the stack after the call, if necessary.
;; We will be pushing parameters in the reverse order.
;; If the function takes no parameters, but returns values, it is the 
;; responsibility of the caller to reserve at least one slot in the stack.
	PUSH	'3'	; push the third parameter
	PUSH	'2'	; push the second parameter
	PUSH	'1'	; push the first parameter
	CALL	func	; call the function
	POP	%R1	; save the return value
	SUB	%SP, 2	; restore the stack, if necessary (still has 2 words)
;	...	
	PUT (%R1)	; use the return value
	STOP

;; FUNCTION
;; It is the responsibility of the function to save the values of ALL registers
;; it plans to use, and to restore them on exit.
func:	
	PUSH	%R1	; save registers
	PUSH	%R2	; save registers
	PUSH	%R3	; save registers
	
	PEEK	%R1, 4+0	; access the 1st argument (4 = 3 saves + 1 PC)
	PEEK	%R2, 4+1	; access the 2nd argument
	PEEK	%R3, 4+2	; access the 3rd argument

			; use arguments
	PUT (%R1)
	PUT (%R2)
	PUT (%R3)

	MOV	%R1, '\n'	
	POKE	%R1, 4+0	; store the return value in the stack

	;; pop them in the reverse order!
	POP	%R3	; restore saved registers
	POP	%R2	; restore saved registers
	POP	%R1	; restore saved registers
	
	RETN
