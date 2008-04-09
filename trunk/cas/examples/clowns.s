#include "config.h"
#define NL '$'

enter:	MOV	%R1, message
again:	MOV	%R0, [%R1]
	CMP	%R0, NL
	JZ	end
	OUT	%R0, ?(IOBASE_TTY + 0)		; to screen
	INC	%R1
	JMP	again
end:	STOP

.align8	/* just for testing purposes */
/*.global*/
message: .string "-- Welcome to CS355! --\n"
	 .string "- Clowns are in town! -\n$"
