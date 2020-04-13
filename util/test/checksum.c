#include <stdio.h>
#include <assert.h>

#include "../checksum.h"

void fletcher16Empty(void) {
	uint8_t data[] = { };
	size_t len = 0;

	uint16 csum = 0x0000;
	
	assert(fletcher16(data, len) == 0);
}

void main(void) {
	fletcher16Empty();

	printf("All tests passed!");
}
