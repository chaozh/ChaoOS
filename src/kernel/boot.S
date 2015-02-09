[org 0x7c00]
KERNEL_OFFSET equ 0x1000
[global _start]
_start:
	; still start at org 0x7c00
	mov bp, 0x9000
	mov sp, bp
	call print_string 
[bits 16]
reset_drive:
	mov ah, 0
	int 13h ; reset drivers
	or ah, ah ; check error code
	jnz reset_drive ; if ah != 0
	;;for debug load code to 0x1000
	;; load kernel code to 0x10000(es:bx = 0x1000:0000)
	mov ax, KERNEL_OFFSET 
	;mov es, ax ; es = 0x1000
	mov bx, ax ; *buffer
	mov si, 1 ; sector 1
	mov cx, 18 ; read 128 sectors, 64KB
_read_sect_loop:
	call read_sect 
	inc si
	add bx, 0x200
	dec cx
	jnz _read_sect_loop
	jmp switch_to_pm
	jmp $

%include "src/kernel/print_string.asm"
;; read_sect(sn, *buf, num) read sector from the disk
;; input: si-logical sector number; 
;;        bx-*buf
;;        cx-num
read_sect:
	push cx
	
	mov ax, si ;sector number
	; ch = cylinder = sn / 36
	mov cl, 36
	div cl
	mov ch, al
	; dh = head = (sn%36) / 18
	mov al, ah
	mov ah, 0
	mov cl, 18
	div cl
	mov dh, al
	; cl = start sector = (sn%36) % 18 + 1	
	mov cl, ah
	inc cl
	; dl = drive = 0
	mov dl, 0
	; raise int 13h read to es:bx
	mov ah, 0x02
	mov al, 1;cl ; ah = 2, al = 1 only time
	int 13h
	;jc disk_error ; if error

	;cmp dh, al ; if AL != DH
	;jne disk_error
	pop cx
	ret

; Switch to protected mode
switch_to_pm:
	xor ax, ax ; clear registers
	mov ds, ax
	mov ss, ax
	mov es, ax

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
	jmp $

[bits 32]
%include "src/kernel/print_string_pm.asm"
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	;mov back stack pointer
	;mov ebp, 0x1000
	;mov esp, ebp

	;cld

	mov ebx, MSG_PROT_MODE
	call print_string_pm
	jmp KERNEL_OFFSET; to entry
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
