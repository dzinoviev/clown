;;; This program is designed for calibrating the Clown CPU's
;;; performance. It measures the time taken by the VM to complete 25
;;; million loops. Each loop consists of 2 instructions, totaling 50
;;; million instructions. On an Intel(R) Core(TM) i5-8400 CPU @
;;; 2.80GHz, it takes approximately 1 second.  When running the native
;;; code (speed.asm), the execution time is around 0.012 seconds.
;;; This indicates that the virtual machine incurs about an 80-fold
;;; slowdown.  Making it similar to the Intel 80386DX CPU, which was
;;; in use from 1986 to 2006.
	
	MOV %R1, 25000000
again:	DEC %R1
	JNZ again
	STOP
