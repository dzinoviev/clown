.code $mycode
	mov %ss, $mystack
	mov %sp, endstack
	
	mov %isr, $isr		; initialize %isr
	
	mov %ds, $isr		; treat $isr as a data segment
	mov %r8, myisr
	add %r8, 2
	mov %es, $segfault	; init the 3rd entry (IRQ3 = div by 0)
	mov %r9, %es
	mov [%r8], %r9		
	div %r1, %r1		; cause an interrupt
	stop

.data $mystack
mystack:	.word[32]
endstack:	.word
