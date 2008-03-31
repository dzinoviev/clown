;; test function
main:
	mov %r1, msg
	
	push %r1		; push the parameter -- optional!
	call puts
	sub %sp, 1		; restore the stack

	push %r1		; push the parameter
	call gets
	sub %sp, 1		; restore the stack -- optional!
	
	push %r1		; push the parameter -- optional!
	call puts
	sub %sp, 1		; restore the stack
	
	stop

msg:	.string "Hello world! Please type something and press <enter>:"
