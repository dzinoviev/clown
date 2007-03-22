#include "config.h"

		MOV	%R1, 1
		OUT	%R1, (IOBASE_TIMER + 0)
		MOV	%R1, 1000
		OUT	%R1, (IOBASE_TIMER + 0)
	HLT

