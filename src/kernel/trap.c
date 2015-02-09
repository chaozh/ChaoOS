#include <types.h>
#include <x86.h>

struct gate_desc idt[IDT_SIZE];
struct idt_desc idt_desc;

//handlers to each No.
static void* handler_routines[IDT_SIZE] = {0,};

void idt_set_gate(uint nr, uint base, ushort sel, uchar type, uchar dpl) {
	idt[nr].base_lo = (base & 0xFFFF);
	idt[nr].base_hi = (base >> 16) & 0xFFFF;
	idt[nr].sel = sel;
	idt[nr].dpl = dpl;
	idt[nr].type = type;
	idt[nr].always0 = 0;
	idt[nr].p = 1;
	idt[nr].sys = 0;
}

static inline void sys_gate(uint nr, uint base) {
	idt_set_gate(nr, base, KERN_CS, STS_TRG, RING3);
}

static inline void intr_gate(uint nr, uint base) {
	idt_set_gate(nr, base, KERN_CS, STS_IG, RING0);
}

static inline void trap_gate(uint nr, uint base) {
	idt_set_gate(nr, base, KERN_CS, STS_TRG, RING0);
}

void _handler() {

}

void set_handler(int nr, int (*func)(struct trap *tf)) {
	handler_routines[nr] = func;
}

void __idt_init() {
	sys_gate(0x80, (uint)_handler);
}

void irq_enable(uchar irq) {
	ushort irq_mask = (port_byte_in(PIC2_DATA)<<8) + port_byte_in(PIC1_DATA);
	irq_mask &= ~(1<<irq);
	port_byte_out(PIC1_DATA, irq_mask);
	port_byte_out(PIC2_DATA, irq_mask >> 8);
}

void irq_init() {
	//init pic for interrupt
	port_byte_out(PIC1, 0x11);
	port_byte_out(PIC2, 0x11);
	port_byte_out(PIC1_DATA, IRQ0);
	port_byte_out(PIC2_DATA, IRQ8);
	port_byte_out(PIC1_DATA, 4);
	port_byte_out(PIC2_DATA, 2);
	port_byte_out(PIC1_DATA, 0x01);
	port_byte_out(PIC2_DATA, 0x01);
	//IRQ mask init
	port_byte_out(PIC1_DATA, 0xFF);
	port_byte_out(PIC2_DATA, 0xFF);
	irq_enable(2); //2 means
}

void lidt(struct idt_desc idtd) {
	asm volatile("lidt %0": : "m"(idtd));
}

void idt_init() {
	idt_desc.base = (uint)&idt;
	idt_desc.limit = (sizeof(struct gate_desc) * IDT_SIZE) - 1;
	// init irq
	irq_init();
	// init idt table
	__idt_init();
	lidt(idt_desc);
}
