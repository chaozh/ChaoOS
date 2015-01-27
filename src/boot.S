[bits 16]
	org 0x7c00
	mov bp, 0x9000
	mov sp, bp
	call print_string 
	jmp switch_to_pm
	jmp $

%include "src/print_string.asm"
%include "src/print_string_pm.asm"
; Switch to protected mode
switch_to_pm:
	cli ; close interrupt
	lgdt [gdt_desc] ; load GDT table
	
	; open A20
	in al, 92h
	or al, 00000010b ;0x2
	out 92h, al	

	mov eax, cr0 ; set cr0 bit0 to 1
	or eax, 0x1
	mov cr0, eax
	
	jmp dword CODE_SEG:init_pm ; to 32-bit code

[bits 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000 ; right place for stack pointer
	mov esp, ebp
	
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	jmp $
; GDT
gdt_start:
gdt_null:
	dd 0, 0
gdt_code:
	dw 0xffff ; Limit(bits 0-15)
	dw 0x0 ; base(0-15)
	db 0x0 ; base(16-23)
	db 10011010b ; 1st falgs, type flags
	db 11001111b ; 2nd flags, Limit(bits 16-19)
	db 0x0 ; base(24-31)
gdt_data:
	dw 0xffff 
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:

gdt_desc:
	dw gdt_end - gdt_start - 1 ; size of GDT,16bits
	dd gdt_start ; address of GDT,32bits

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0

times 510-($-$$) db 0
dw 0xaa55
