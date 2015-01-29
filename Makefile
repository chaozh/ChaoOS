#Programs, flags, etc.
ASM = nasm
CC = gcc
LD = ld
#This Program
CHAOBOOT = bin/kernel.bin

C_SOURCES = $(wildcard src/*.c)

# Convert the *.c to *.o
OBJ = ${C_SOURCES:.c=.o}

run: bochs

bochs: build 
	bochs -q -f bochsrc
debug: build
	bochs -dbg -q -f bochsrc
build: 
	echo 

all: $(CHAOBOOT)
buildimg: 
	dd if=bin/kernel.bin of=bin/a.img bs=512 count=1 conv=notrunc
#Build the kernel binary, $^ = {*.o}
$(CHAOBOOT): src/boot.o ${OBJ}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
#Build the kernel object file
%.o : %.c
	$(CC) -m32 -ffreestanding -c $< -o $@
#Build the kernel entry object file
%.o : %.S
	$(ASM) $< -f elf -o $@

clean:
	rm src/*.o
	rm -f $(CHAOBOOT)
