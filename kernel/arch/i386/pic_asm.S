PIC1 equ 0x20
PIC2 equ 0x28
EOI equ 0x20

PIC_AMOUNT equ 16

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


%macro isr_pic_stub 1
isr_stub_pic_%+%1:

	pusha
	push ds
    push es
    push fs
    push gs

	push dword %1

	call pic_handler

	add esp, 4

	pop gs 
	pop fs 
	pop es 
	pop ds 
	popa

	mov al, EOI
	out PIC1, al 
	%if %1>7
		out PIC2, al
	%endif

    iret 
%endmacro

extern pic_handler

%assign i 0 
%rep    PIC_AMOUNT 
isr_pic_stub  i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

global pic_stub_table
pic_stub_table:
%assign i 0 
%rep    PIC_AMOUNT 
    dd isr_stub_pic_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

