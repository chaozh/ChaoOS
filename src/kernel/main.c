#include <types.h>
#include <x86.h>

struct tss_desc tss;

struct seg_desc gdt[GDT_SIZE] = {{0, }, };//init
struct gdt_desc gdt_desc;

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
	//load tss
	asm volatile("ltr %%ax": :"a"(_TSS));
}

void main() {
	cls(); 
	print("start kernel\n");	
	gdt_init(); 	
	print("init gdt\n");
	idt_init();	
	print("init ldt\n");
	keybd_init();	
	print("keybd init\n");
}
