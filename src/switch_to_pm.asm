[bits 16]
; Switch to protected mode
switch_to_pm:
	cli ; close interrupt
	lgdt [gdt_descriptor] ; load GDT table
	
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
	
	call BEGIN_PM
