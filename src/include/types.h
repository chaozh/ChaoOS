#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#define NULL 0

//const
#define GDT_SIZE 6
#define IDT_SIZE 256

#define PAGE_SIZE 4096
#define KSTACK0 0x9000

#define NPROC 64
#endif
