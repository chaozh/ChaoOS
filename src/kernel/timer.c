#include <types.h>
#include <x86.h>

#define HZ 100

static volatile uint jiffies = 0; //tick TODO: need wrapp
static volatile uint timestamp = 0;

int do_timer(struct trap* tf) {
	jiffies++; //danger
	//
	return 0;
}
//get system time
uint cmos_time() {
	uint r;
	
	return r;
}

uint time() {
	return timestamp;
}

void timer_init() {
	uint di = 11933180/HZ;
	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, (uchar)(di&0xff));
	port_byte_out(0x40, (uchar)(di>>8));
	//
	timestamp = cmos_time();
	//
	irq_enable(0);
	set_handler(IRQ0, &do_timer);
}
