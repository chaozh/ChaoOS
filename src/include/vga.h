#ifndef VGA_H
#define VGA_H

#define WHITE_ON_BLACK 0x0f
#define WHITE 0x07
#define BLACK 0x00

void print_char(char charatcter, int col, int row, char attr);
void print_at(char* msg, int col, int row);
void print(char* msg);

#endif
