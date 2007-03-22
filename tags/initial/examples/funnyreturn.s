#include "config.h"

	jmp end

begin:	mov %r1, '$'
	out	%R1, (IOBASE_TTY + 0)		; to screen
	stop

end:	mov %r0, begin
	push %r0
	retn