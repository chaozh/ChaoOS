#include <types.h>
#include <x86.h>

#include <tty.h>
#include <vga.h>
#include <keybd.h>

static uint mode = 0;

//check ctrl, alt and shift status
int special(char ch) {
	char tmp = ch & 0x7f;
	if (mode & E0ESC){
		switch(tmp) {
		case 0x1D: return CTRL;
		case 0x38: return ALT;
		}
	} else {
		switch(tmp) {
		case 0x2A:
		case 0x36: return SHIFT;
		case 0x1D: return CTRL;
		case 0x38: return ALT;
		}
	}
	return 0;
}

int do_keybd_intr(struct trap *tf){
	uchar tmp, ch, m;
	uchar* map = keybd_map;
	//check keyboard status if no data
	if ((port_byte_in(KB_STAT) && KB_STAT_DIB) == 0) {
		return -1;
	}
	tmp = port_byte_in(KB_DATA);
	
	//ignore capfslock yet
	if((tmp & 0x7f)==0x3A) {
		return 0;
	}
	// check ESC
	if(tmp == 0xE0)
		mode |= E0ESC;
	//check shift etc.
	if((m = special(tmp))) {
		if(tmp & 0x80)
			mode &= ~m;
		else
			mode |= m;
	}
	map = (mode & SHIFT)?shift_map:keybd_map;
	ch = map[tmp & 0x7f];
	
	if(mode & CTRL){
		switch(ch){
		case 'c': ch = CINTR;
		case 'd': ch = CEOF;
		case 'x': ch = CKILL;
		case '\\': ch = CQUIT;
		case 'z': ch = CSUSP;
		}
	}
	// on pressed key
	if ((tmp & 0x80)==0 && ch != '\0') {
		//tty_input(&tty[0], ch);
		print_char(ch, -1, -1, WHITE_ON_BLACK);
	} else {
		mode &= ~E0ESC; 
	}

	return 0;
}

void keybd_init() {
	irq_enable(1);
	set_handler(IRQ1, &do_keybd_intr);	
}
