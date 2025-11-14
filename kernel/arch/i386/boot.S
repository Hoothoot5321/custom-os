bits 32
extern _init
extern kernel_main
extern _kernel_end

global VIRTUAL_OFFSET
global PAGE_SIZE

ALIGNMENT equ    1<<0             
MEMINFO equ 1<<1             
FLAGS equ   ALIGNMENT | MEMINFO 
MAGIC equ   0x1BADB002     
CHECKSUM equ -(MAGIC + FLAGS) 
VIRTUAL_OFFSET equ 0xC0000000 
PAGE_SIZE equ 0x1000

section .multiboot.data
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .stack
stack_bottom:
resb 16384 
stack_top:

section .bss
align PAGE_SIZE 
boot_page_directory:
resb PAGE_SIZE 
boot_page_table1:
resb PAGE_SIZE 

section .multiboot.text
global _start
_start:

	mov ecx, eax
	mov edx, ebx 

	mov esi, boot_page_table1 - VIRTUAL_OFFSET
	mov edi,esi 
	mov esi, 0
map_pre_kernel:
	mov eax, esi
	or eax, 0x003
	mov [edi], eax 

	add esi, PAGE_SIZE
	add edi, 4
	mov eax, _kernel_end - VIRTUAL_OFFSET
	cmp eax, esi
	jge map_pre_kernel

end_map:
	mov edi,boot_page_directory -VIRTUAL_OFFSET 
	mov eax, boot_page_table1  - VIRTUAL_OFFSET 
	or eax, 0x003
	mov [edi],eax 
	mov [edi + 768*4],eax 

	mov eax, (boot_page_directory - VIRTUAL_OFFSET)
	mov cr3, eax

	mov eax, cr0
	or eax,0x80010000 
	mov cr0, eax

	jmp start_kernel 

section .text

start_kernel:
	mov esp, stack_top
	mov eax,boot_page_directory

	mov dword [eax], 0

	mov eax, cr3
	mov cr3, eax

	mov eax, ecx
	mov ebx, edx

	push eax
	push ebx

	call _init

	pop ebx
	pop eax

	push eax
	push ebx

	call kernel_main


	cli
idle_loop:	
	hlt
	jmp idle_loop 
_start_end:
