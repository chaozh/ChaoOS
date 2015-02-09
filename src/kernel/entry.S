; jump into the kernel's entry function
[bits 32]
[global _start]
_start:
	mov ebx, ENTRY_MSG
	call print_string_pm
	jmp ready
	jmp $
%include "src/kernel/print_string_pm.asm"
[extern main]
ready:
	call main
	jmp $

ENTRY_MSG db "Welcome to ChaoOS", 0
