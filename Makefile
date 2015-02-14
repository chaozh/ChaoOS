#Programs, flags, etc.
ASM = nasm
CC = gcc
LD = ld
CINC := -Isrc/include
#This Program
CHAOBOOT = bin/ChaoOS.img
CHAOELF = bin/kernel.elf
CHAOSYM = bin/kernel.sym
C_HEADERS = $(wildcard src/include/*.h)
C_SOURCES = $(wildcard src/**/*.c)

# Convert the *.c to *.o
OBJ = ${C_SOURCES:.c=.o}

all: $(CHAOBOOT) $(CHAOSYM)
run: bochs

bochs: build 
	bochs -q -f .bochsrc
debug: build
	bochs-dbg -q -f .bochsrc-dbg
build: 
	echo 

$(CHAOBOOT): bin/boot.bin bin/kernel.bin
	cat $^ > $(CHAOBOOT)
#buildimg: 
#	dd if=$(CHAOBOOT) of=bin/a.img bs=512 count=1 conv=notrunc
#Build the kernel binary, $^ = {*.o}
#bin/kernel.bin: src/kernel/entry.o ${OBJ}
#	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
bin/kernel.bin: $(CHAOELF)
	objcopy -R .pdr -R .comment -R .note -S -O binary $< $@
$(CHAOELF): src/kernel/entry.o ${OBJ}
	$(LD) $^ -m elf_i386 -o bin/kernel.elf -Ttext 0x1000
$(CHAOSYM): $(CHAOELF) 
	(nm $< | sort) > $@
	
#Build the kernel object file
%.o : %.c
	$(CC) -m32 $(CINC) -g -ffreestanding -c $< -o $@
#Build the kernel entry object file
%.o : %.S
	$(ASM) $< -f elf -o $@

bin/boot.bin : src/kernel/boot.S
	$(ASM) $< -f bin -o $@

clean:
	rm src/kernel/*.o src/drivers/*.o
	rm -f $(CHAOBOOT) $(CHAOELF) $(CHAOSYM)
	rm -f bin/boot.bin bin/kernel.bin 

.PHONY: run all bochs build debug clean
