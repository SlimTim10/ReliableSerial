#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

uint16_t fletcher16(uint8_t *data, size_t length) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	size_t i;

	for (i = 0; i < length; i++) {
		sum1 = (sum1 + data[i]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}

	return (sum2 << 8) | sum1;
}

bool check(uint8_t *data, size_t length) {
	return fletcher16(data, length) == 0x0000;
}

/* The size of the dst buffer must be at least 2 bytes larger than srcLength */
uint8_t *withCheckBytes(uint8_t *src, uint8_t *dst, size_t srcLength) {
	uint16_t csum;
	uint8_t c0, c1, f0, f1;

	csum = fletcher16(src, srcLength);
	f0 = csum & 0xFF;
	f1 = (csum >> 8) & 0xFF;
	c0 = 0xFF - ((f0 + f1) % 0xFF);
	c1 = 0xFF - ((f0 + c0) % 0xFF);

	memcpy(dst, src, srcLength);
	dst[srcLength] = c0;
	dst[srcLength + 1] = c1;
	
	return dst;
}

/* The size of the dst buffer must be at least 2 bytes less than srcLength */
uint8_t *withoutCheckBytes(uint8_t *src, uint8_t *dst, size_t srcLength) {
	if (srcLength < 2) {
		return dst;
	}

	memcpy(dst, src, srcLength - 2);
	
	return dst;
}

#endif
