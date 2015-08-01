#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uchar; //1b
typedef unsigned short ushort; //2b
typedef unsigned int uint; //4b

typedef ushort uint16_t;
typedef uint uint32_t;
typedef unsigned long long uint64_t;

#define NULL 0

//const
#define GDT_SIZE 6
#define IDT_SIZE 256

#define PAGE_SIZE 4096
#define KSTACK0 0x9000

#define NPROC 64
#endif
