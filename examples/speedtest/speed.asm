;;; This program is designed for calibrating the Clown CPU's
;;; performance. See speed.s for more info information.

	GLOBAL main
main:
	MOV eax, 25000000
again:	DEC eax
	JNZ again
	
	RET

;;; To execute:
;;; 
;;; nasm -f elf64 speed.asm -o speed.o
;;; gcc -o speed speed.o	
;;; time ./speed
