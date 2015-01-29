nasm src/boot.S -o bin/bootS.bin
dd if=bin/bootS.bin of=bin/a.img bs=512 count=1 conv=notrunc

