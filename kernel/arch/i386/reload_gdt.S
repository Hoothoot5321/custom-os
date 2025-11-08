bits 32

global reloadSegments 
extern gdt_ptr

reloadSegments:
   jmp   0x08:reload_CS 
reload_CS:
   mov   ax, 0X10 
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret
