	org 0x7c00
	mov bp, 0x9000
	mov sp, bp
	call switch_to_pm ; we never return to next line
	jmp $

%include "src/gdt.asm"
%include "src/print_string_pm.asm"
%include "src/switch_to_pm.asm"

[bits 32]
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	jmp $
MSG_PROT_MODE db "Landed in 32-bit Protected Mode"

times 510-($-$$) db 0
dw 0xaa55
