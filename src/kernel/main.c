#include <types.h>
#include <x86.h>
#include <sched.h>

struct seg_desc gdt[GDT_SIZE] = {{0, }, };//init
struct gdt_desc gdt_desc;

void init();

void set_seg(struct seg_desc *seg, uint base, uint limit, uint dpl, uint type){
	seg->limit_lo = ((limit) >> 12) & 0xffff;
	seg->base_lo = (base) & 0xffff;
	seg->base_mi = ((base) >> 16) & 0xff;
	seg->type = type;
	seg->s = 1;
	seg->dpl = dpl;
	seg->present = 1;
	seg->limit_hi = (uint)(limit) >> 28;
	seg->avl = 0;
	seg->r = 0;
	seg->db = 1;
	seg->g = 1;
	seg->base_hi = (base) >> 24;
}

void set_tss(struct seg_desc *seg, uint base) {
	set_seg(seg, base, 0, RING0, STS_TA);
	seg->limit_lo = 0x68; //68 means 4G address space 
	seg->s = 0; //system seg
}

void gdt_init() {
	set_seg(&gdt[1], 0, 0xffffffff, RING0, STA_X | STA_R);
	set_seg(&gdt[2], 0, 0xffffffff, RING0, STA_W);
	set_seg(&gdt[3], 0, 0xffffffff, RING3, STA_X | STA_R);
	set_seg(&gdt[4], 0, 0xffffffff, RING3, STA_W);
	set_tss(&gdt[TSS0], (uint)&tss);
	gdt_desc.base = (uint)&gdt;
	gdt_desc.limit = (sizeof(struct seg_desc) * GDT_SIZE) - 1;		
	//load gdt
	asm volatile("lgdt %0": :"m"(gdt_desc));
	//load tss for process exange
	asm volatile("ltr %%ax": :"a"(_TSS));
}

static inline void sti() {
	asm volatile("sti");
}

void start_kernel() {
	cls(); 
	gdt_init(); 	
	print("init gdt\n");
	idt_init();// trap_init & irq_init	
	print("init ldt\n");
	//process.c: init zero process also called idle
	swapper_init();		
	//
	keybd_init();	
	print("keybd init\n");
	
	print("start kernel\n");
	sti();	 
	/*kernel_thread(init, NULL, CLONE_FS | CLONE_SIGHAND);
	for(;;){
		sti();
		schedule();
	}*/	
}

void init() {
	print("start init thread\n");
	for(;;);
}
