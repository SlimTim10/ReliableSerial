#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../checksum.h"

void fletcher16Empty(void) {
	uint8_t data[] = { };
	size_t len = 0;
	uint16_t csum = 0x0000;
	
	assert(fletcher16(data, len) == csum);
}

void fletcher16Small(void) {
	uint8_t data[] = { 0x01, 0x02 };
	size_t len = 2;
	uint16_t csum = 0x0403;

	assert(fletcher16(data, len) == csum);
}

void fletcher16Simple(void) {
	uint8_t data[] = "abcdefgh";

	size_t len1 = 5;
	uint16_t csum1 = 0xC8F0;

	size_t len2 = 6;
	uint16_t csum2 = 0x2057;

	size_t len3 = 8;
	uint16_t csum3 = 0x0627;


	assert(fletcher16(data, len1) == csum1);
	assert(fletcher16(data, len2) == csum2);
	assert(fletcher16(data, len3) == csum3);
}

void checkEmpty(void) {
	uint8_t data[] = { };
	size_t len = 0;

	assert(check(data, len) == true);
}

void checkSmall(void) {
	uint8_t dataGood[] = { 0x01, 0x02, 0xF8, 0x04 };
	uint8_t dataBad[] = { 0x01, 0x02, 0x00, 0x00 };
	size_t len = 4;

	assert(check(dataGood, len) == true);
	assert(check(dataBad, len) == false);
}

void withCheckBytesSmall(void) {
	enum {
		dataLen = 2,
		bufLen = 255,
		expectedLen = 4,
	};
	
	uint8_t data[dataLen] = { 0x01, 0x02 };
	uint8_t buf[bufLen];
	uint8_t *after = withCheckBytes(data, buf, dataLen);
	uint8_t expected[expectedLen] = { 0x01, 0x02, 0xF8, 0x04 };

	assert(memcmp(after, expected, expectedLen) == 0);
}

void withoutCheckBytesEmpty(void) {
	enum {
		dataLen = 0,
		bufLen = 0,
		expectedLen = 0,
	};
	
	uint8_t data[dataLen] = { };
	uint8_t buf[bufLen];
	uint8_t *after = withoutCheckBytes(data, buf, dataLen);
	uint8_t expected[expectedLen] = { };

	assert(memcmp(after, expected, expectedLen) == 0);
}

void withoutCheckBytesSmall(void) {
	enum {
		dataLen = 4,
		bufLen = 2,
		expectedLen = 2,
	};
	
	uint8_t data[dataLen] = { 0x01, 0x02, 0xF8, 0x04 };
	uint8_t buf[bufLen];
	uint8_t *after = withoutCheckBytes(data, buf, dataLen);
	uint8_t expected[expectedLen] = { 0x01, 0x02 };

	assert(memcmp(after, expected, expectedLen) == 0);
}

void main(void) {
	fletcher16Empty();
	fletcher16Small();
	fletcher16Simple();

	checkEmpty();
	checkSmall();

	withCheckBytesSmall();
	withoutCheckBytesEmpty();
	withoutCheckBytesSmall();

	printf("All tests passed!");
}
