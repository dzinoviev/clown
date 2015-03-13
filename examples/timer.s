;; This program WILL cause a double fault

#include <hardware.h>

		MOV	%R1, 1
		OUT	%R1, ?(IOBASE_TIMER + 0)
		MOV	%R1, 1000
		OUT	%R1, ?(IOBASE_TIMER + 0)
	HLT

