bits 32
extern _init
extern kernel_main

ALIGNMENT equ    1<<0             
MEMINFO equ 1<<1             
FLAGS equ   ALIGNMENT | MEMINFO 
MAGIC equ   0x1BADB002     
CHECKSUM equ -(MAGIC + FLAGS) 

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 
stack_top:

section .text
global _start
_start:
	mov esp, stack_top

	call _init

	call kernel_main

	cli
idle_loop:	
	hlt
	jmp idle_loop 
_start_end:

_start_size equ _start_end - _start
