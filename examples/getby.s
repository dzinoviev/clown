;;; This program extracts bytes from a word and packs bytes into the word
	MOV %R2, 0x12345678
	MOV %R1, %R2(2)
	INC %R1
	MOV %R2(2), %R1
	STOP
