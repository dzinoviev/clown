;; test function
.global main:
	mov %r1, msg
	
	push %r1		; push the parameter -- optional!
	call puts
	inc %sp		; restore the stack

	push %r1		; push the parameter
	call gets
	inc %sp		; restore the stack -- optional!
	
	push %r1		; push the parameter -- optional!
	call puts
	inc %sp		; restore the stack
	
	retn	

msg:	.string "Hello world! Please type something and press <enter>:"
