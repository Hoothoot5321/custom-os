bits 32

section    .text
    global set_idt 
idtr:
	dw 0
	dd 0

global idt_reload
set_idt:
	mov ax, [esp + 4]
	mov word [idtr], ax

	mov eax, [esp + 8]
	mov dword [idtr+2], eax
    lidt [rdi]
    ret
