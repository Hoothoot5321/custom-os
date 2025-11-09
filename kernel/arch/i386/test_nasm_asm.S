
section .data
	format db "Hello %u %u",10,0

section .text

extern printf
global nasm_print

nasm_print:
	mov ecx, [esp + 8]
	mov eax, [esp + 4]

	push ebx

	mov ebx, ecx
	add ebx, eax

	push ecx
	push eax

	lea eax, [format]
	push eax

	call printf
	
	add esp, 12

	mov eax, ebx

	pop ebx

	ret
	

