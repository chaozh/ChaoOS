;Directly using video memory
[bits 32]
; Define Constants
VIDEO_MEM equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
	pusha
	mov edx, VIDEO_MEM
print_string_pm_loop:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK

	cmp al, 0
	je done

	mov [edx], ax
	add ebx, 1
	add edx, 2
	jmp print_string_pm_loop
done:
	popa
	ret
