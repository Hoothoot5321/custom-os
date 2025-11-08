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
    lidt [idtr]
    ret


%macro isr_err_stub 2
isr_stub_%+%1:
	cli
	
	%if %2=0
		push dword 0 
	%endif

	pusha
	push ds
    push es
    push fs
    push gs

	push dword %1
	mov eax, esp
	push eax
	%if %2=0
		call no_error_code_exception_handler 
	%else
		call error_code_exception_handler 
	%endif
	add esp, 8

	pop gs 
	pop fs 
	pop es 
	pop ds 
	popa

	add esp, 4

	sti

    iret 
%endmacro


extern no_error_code_exception_handler
extern error_code_exception_handler

isr_err_stub 0, 0
isr_err_stub 1, 0
isr_err_stub 2, 0
isr_err_stub 3, 0
isr_err_stub 4, 0
isr_err_stub 5, 0
isr_err_stub 6, 0
isr_err_stub 7, 0
isr_err_stub 8, 1
isr_err_stub 9, 0
isr_err_stub 10, 1
isr_err_stub 11, 1
isr_err_stub 12, 1
isr_err_stub 13, 1
isr_err_stub 14, 1
isr_err_stub 15, 0
isr_err_stub 16, 0
isr_err_stub 17, 1
isr_err_stub 18, 0
isr_err_stub 19, 0
isr_err_stub 20, 0
isr_err_stub 21, 1
isr_err_stub 22, 0
isr_err_stub 23, 0
isr_err_stub 24, 0
isr_err_stub 25, 0
isr_err_stub 26, 0
isr_err_stub 27, 0
isr_err_stub 28, 0
isr_err_stub 29, 0
isr_err_stub 30, 0
isr_err_stub 31, 0

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep
