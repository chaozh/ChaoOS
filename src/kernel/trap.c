#include <types.h>
#include <x86.h>
//from entry.S
extern uint handlers[IDT_SIZE];

struct gate_desc idt[IDT_SIZE];
struct idt_desc idt_desc;

//handlers to each No.
static void* interrupt[IDT_SIZE] = {0,};

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

void set_handler(int nr, int (*func)(struct trap *tf)) {
	interrupt[nr] = func;
}

void trap_init() {
	int i;
	for(i=0; i<32; i++){
		trap_gate(i, handlers[i]);
		printk("handler[%d]:%d\n",i,&handlers[i]);
	}
	for(i=32; i<48; i++)
		intr_gate(i, handlers[i]);
	sys_gate(0x03, handlers[0x03]); // int 3
	sys_gate(0x04, handlers[0x04]); // overflow
	sys_gate(0x80, handlers[0x05]); // round
	sys_gate(0x80, handlers[0x80]);
}

void irq_enable(uchar irq) {
	ushort irq_mask = (port_byte_in(PIC2_DATA)<<8) + port_byte_in(PIC1_DATA);
	irq_mask &= ~(1<<irq);
	port_byte_out(PIC1_DATA, irq_mask);
	port_byte_out(PIC2_DATA, irq_mask >> 8);
}

void init_IRQ() {
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

/**
 * Common handler for all IRQ reqs 
 *
 */
void do_IRQ(struct trap *tf) {
	void (*func)(struct trap *tf);
	print("do_IRQ\n");
	func = interrupt[tf->int_no];
	if(tf->int_no < 32){
		//trap
		if(func) 
			func(tf);
		
	} else {
		//irq, syscall
		if(func)
			func(tf);
	}	
	//only schedule when trying return to user mode
	//if((tf->cs & 3) == RING3)
	//	schedule();
}

void idt_init() {
	idt_desc.base = (uint)&idt;
	idt_desc.limit = (sizeof(struct gate_desc) * IDT_SIZE) - 1;
	// init irq
	init_IRQ();
	// init idt table
	trap_init();
	lidt(idt_desc);
}
