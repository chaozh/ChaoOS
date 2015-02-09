#include <types.h>
#include <vga.h>

#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25
#define MAX_COLS 80
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int get_screen_offset(int col, int row) {
	return (row*MAX_COLS + col)*2;
}

int get_cursor() {
	//reg 14 & 15
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	// convert charater to vga size
	return offset*2;
}

void set_cursor(int offset) {
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (uchar)offset >> 8);
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (uchar)offset);
}

int handle_scrolling(int cursor_offset) {
	//if the cursor is within the screen
	if (cursor_offset < MAX_ROWS*MAX_COLS*2)
		return cursor_offset;
	//copy
	int i;
	for(i=0; i<MAX_ROWS; ++i){
		memcpy(get_screen_offset(0, i)+VIDEO_ADDRESS, get_screen_offset(0, i-1)+VIDEO_ADDRESS, MAX_COLS*2);
	}
	//blank the last line
	char* last_line = (char*)(get_screen_offset(0, MAX_ROWS-1)+VIDEO_ADDRESS);
	for(i=0; i<MAX_COLS; ++i)
		last_line[i]=0;
	//Move offset back one row to the last line of the screen
	cursor_offset -= 2*MAX_COLS;
	return cursor_offset;
}

void print_char(char character, int col, int row, char attr) {
	uchar *vidmem = (uchar*)VIDEO_ADDRESS;
	
	if(!attr) {
		attr = WHITE_ON_BLACK;	
	}
	int offset;
	if(col >= 0 && row >=0) {
		offset = get_screen_offset(col, row);
	} else {
		offset = get_cursor();
	}
	// start from the new line 	
	if(character == '\n') {
		int row = offset / (2*MAX_COLS);
		offset = get_screen_offset(MAX_COLS-1, row);
	} else {
		vidmem[offset] = character;
		vidmem[offset+1] = attr;
	}
	// next char position	
	offset += 2;
	offset = handle_scrolling(offset);
	//update cursor position
	set_cursor(offset);
}

void print_at(char* msg, int col, int row) {
	if(col >= 0 && row >= 0){
		set_cursor(get_screen_offset(col, row));
	}
	//FIX:wq
	int i = 0;
	while(msg[i] != '\0'){
		print_char(msg[i++], col, row, WHITE_ON_BLACK);	
	}
}

void print(char* msg) {
	print_at(msg, -1, -1);
}
/*
//a simple imp need change!
void printk(char* fmt, ...) {
	char c;
	int n;
	int* adx = (int*)(void*)(&fmt) + 1;
loop:
	while((c = *fmt++) != '%'){
		if(c=='\0') return;
		putchar(c);
	}
	c = *fmt++;
	//deal with %d or %l
	if(c=='d' || c== 'l'){
		n = *adx;
		if (n<0) {
			putchar('-');
			n *= -1;
		}
		printn(n, 10); //???
	}
	if(c=='o' || c=='x')
		printn(*adx, c=='o'?8:16);
	if(c=='c')
		printn(*adx);
	if(c=='s')
		print((char*)*adx); //puts()
	adx++;
	goto loop;	
}
*/
void cls() {
	memset(VIDEO_ADDRESS, ' '|WHITE<<8, 2*MAX_ROWS*MAX_COLS);
	set_cursor(get_screen_offset(0, 0));
}
