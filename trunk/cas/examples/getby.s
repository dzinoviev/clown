	mov %r2, 0x12345678
	mov %r1, %r2(2)
	inc %r1
	mov %r2(2), %r1
	stop
