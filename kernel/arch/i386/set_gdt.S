bits 32
section .data
    align 2               
gdtr:
    dw 0                
    dd 0               

section    .text
    global set_gdt
set_gdt:
    mov ax,[esp+4]
    mov word [gdtr],ax

    mov eax,[esp+8]
    mov dword [gdtr+2],eax
    lgdt [gdtr]

    ret

