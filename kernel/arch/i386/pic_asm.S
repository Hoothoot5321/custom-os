PIC1 equ 0x20
PIC2 equ 0x28
EOI equ 0x20

PIC_AMOUNT equ 16

global outb
global outw
global inb
global inw
global io_wait
outb:
	mov dx, [esp+4]
	mov al, [esp+8]

	out dx, al

	ret

outw:
	mov dx, [esp+4]
	mov ax, [esp+8]

	out dx, ax

	ret


inb:
	xor eax, eax
	mov dx, [esp+4]

	in al, dx

	ret

inw:
	xor eax, eax
	mov dx, [esp+4]

	in ax, dx

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
	%if %1>=8
		out PIC2, al
	%endif
	out PIC1, al 

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

