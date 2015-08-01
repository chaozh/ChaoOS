#include <types.h>
static inline void io_delay() {
	__asm__("jmp 1f; 1: jmp 1f; 1:");
}

uchar port_byte_in(ushort port) {
	uchar result;
	// "=a" means: put AL reg in variable result
	// "d" means: load EDX with port
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	io_delay();
	return result;
}

void port_byte_out(ushort port, uchar data) {
	//all loads op
	__asm__("out %%al, %%dx": :"a"(data), "d" (port));
	io_delay();
}

ushort port_word_in(ushort port) {
	ushort result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	io_delay();
	return result;
}

void port_word_out(ushort port, ushort data) {
	__asm__("out %%ax, %%dx": :"a"(data), "d" (port));
	io_delay();
}
