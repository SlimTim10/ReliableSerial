#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

uint16_t fletcher16(uint8_t *data, size_t count) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	size_t i;

	for (i = 0; i < count; i++) {
		sum1 = (sum1 + data[i]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}

	return (sum2 << 8) | sum1;
}

#endif
