;;; This program can be used to calibrate the Clown CPU
;;; How long does it take the host system to execute 10 mln loops?
;;; Each loop contains 2 instructions
;;; Takes 2 sec on my AMD Athlon(tm) 64 Processor 3500+ 1000 MHz
;;; This corresponds to 10 MHz -- the slowdown by the factor of 100
	mov %r1, 20000000
again:	dec %r1
	jnz again
	stop
