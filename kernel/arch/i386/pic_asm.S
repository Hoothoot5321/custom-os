
global outb
global inb
global io_wait
outb:
	mov dx, [esp+4]
	mov al, [esp+8]

	out dx, al

	ret

inb:
	xor eax, eax
	mov dx, [esp+4]

	in al, dx

	ret

io_wait:
	mov al, 0
	out 0x80,al
	ret

