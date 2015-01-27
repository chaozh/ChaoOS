; GDT
gdt_start:

gdt_null:
	dd 0x0
	dd 0x0
gdt_code:
	dw 0xffff ; Limit(bits 0-15)
	dw 0x0 ; base(0-15)
	dw 0x0 ; base(16-23)
	db 10011010b ; 1st falgs, type flags
	db 11001111b ; 2nd flags, Limit(bits 16-19)
	db 0x0 ; base(24-31)
gdt_data:
	dw 0xffff 
	dw 0x0
	dw 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; size of GDT,16bits
	dd gdt_start ; address of GDT,32bits

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
