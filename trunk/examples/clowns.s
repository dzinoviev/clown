#include <clown/config.h>
#define NL '$'

	MOV	%R1, message
again:	MOV	%R0, [%R1]
	OUT	%R0, (IOBASE_TTY + 0)		; to screen
	INC	%R1
	CMP	%R0, NL
	JNZ	again
	STOP
message: .string "-- Welcome to CS355! --\n"
	 .string "- Clowns are in town! -\n$"
