;; This program prints an ASCII representation of a number in %r1

#include <hardware.h>

#define DEC 10
	MOV %R1, 1234
	CALL printf
	STOP

printf:	MOV  %R10, DEC
	PUSH %R10
	XOR  %R0, %R0
	MOV %R11, '0'

	CMP %R1, %R0
	JZ zero
divide:	
	MOV %R2, %R1
	DIV %R2, %R10
	MUL %R2, %R10
	MOV %R4, %R1
	SUB %R4, %R2
	PUSH %R4
	DIV %R1, %R10
	CMP %R1, %R0
	JNZ divide

do_print:
	POP %R1
	CMP %R1, %R10
	JZ DONE
	
zero:	ADD %R1, %R11
	OUT	%R1, ?(IOBASE_TTY + 0)		; to screen	
	JMP do_print
done:	RETN
