#include <types.h>
#include <string.h>

void* memcpy(void *dest, void *src, uint count) {
	char *sp = (char *)src;
	char *dp = (char *)dest;
	int i;
	for (i=0; i<count; ++i) {
		*dp++ = *sp++;
	}
	return dest;
}

void* memset(void *dest, int val, uint count) {
	int *dp = (int *)dest;
	int i;
	for(i=0; i<count; ++i){
		*dp++ = val;
	}
	return dest;
}
