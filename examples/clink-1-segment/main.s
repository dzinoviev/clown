;; test function
.GLOBAL main:
	MOV %R1, msg
	
	PUSH %R1		; push the parameter
	CALL puts	
	INC %SP			; restore the stack
	
	PUSH %R1		; push the parameter
	CALL gets
	INC %SP			; restore the stack
	
	PUSH %R1		; push the parameter
	CALL puts
	INC %SP			; restore the stack
	
	RETN	

msg:	.STRING "Hello world! Type something and press <enter>:"
