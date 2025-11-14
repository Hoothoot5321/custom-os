MAX_INTERRUPT_AMOUNT equ 256

%macro isr_default_stub 1
isr_stub_default_%+%1:

	pusha
	push ds
    push es
    push fs
    push gs

	push dword %1

	call default_idt_handler 

	add esp, 4

	pop gs 
	pop fs 
	pop es 
	pop ds 
	popa

    iret 
%endmacro

extern default_idt_handler 

%assign i 0 
%rep   MAX_INTERRUPT_AMOUNT 
isr_default_stub i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

global default_stub_table
default_stub_table:
%assign i 0 
%rep   MAX_INTERRUPT_AMOUNT 
    dd isr_stub_default_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

