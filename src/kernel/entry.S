; jump into the kernel's entry function
[bits 32]
[global _start]
_start:
	mov ebx, ENTRY_MSG
	call print_string_pm
	jmp ready
	jmp $
%include "src/kernel/print_string_pm.asm"
[extern start_kernel]
ready:
	call start_kernel
	jmp $


[global do_switch]
do_switch:
	mov eax, dword [esp+4]	;; new stack
	pop dword [eax]		;; *old
	mov dword [eax+4], esp
	mov dword [eax+8], ebx
	mov dword [eax+12], ecx
	mov dword [eax+16], edx
	mov dword [eax+20], esi
	mov dword [eax+24], edi
	mov dword [eax+28], ebp

	mov eax, dword [esp+4] ;; load new
	
	mov ebp, dword [eax+28]
	mov edi, dword [eax+24]
	mov esi, dword [eax+20]
	mov edx, dword [eax+16]
	mov ecx, dword [eax+12]
	mov ebx, dword [eax+8]
	mov esp, dword [eax+4]
	push dword [eax]
	ret
	
ENTRY_MSG db "Welcome to ChaoOS", 0

[global restore_all]
[extern do_IRQ]
common_interrupt:
	sti
	pusha
	push ds
	push es
	push fs
	push gs
	
	mov ax, 0x10
	mov ds, ax ;; usr seg selector
	mov es, ax
	
	mov eax, esp ; esp is the address to param struct trap* tf
	push eax
	mov eax, do_IRQ ; call do_IRQ() in trap.c
	call eax
	pop eax
restore_all:
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8 ;;
	iret

COMMON_INTERRUPT_ENTER db "enter common_interrupt",0

%macro handler_common_init 1 ;accept one param 
	handlers%1:
		%if %1 != 17 && %1 != 30 && (%1 < 8 || %1 > 14)
			push dword 0
		%endif
		push dword %1
		mov ebx, COMMON_INTERRUPT_ENTER
		call print_string_pm 
		jmp common_interrupt
%endmacro

%assign i 0
%rep 129
	handler_common_init i
	%assign i i+1
%endrep

%macro handlers_init 1
	mov ebx, COMMON_INTERRUPT_ENTER
	call print_string_pm 
	dd handlers%1
%endmacro

; vector table
[section .data]
[global handlers]
handlers:
	%assign i 0
	%rep 129
		handlers_init i
		%assign i i+1
	%endrep
