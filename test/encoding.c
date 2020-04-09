#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../encoding.h"

uint8_t buf[COBS_DECODE_MAX_LENGTH];

void encodeSimple(void) {
	uint8_t before[COBS_ENCODE_MAX_LENGTH] = {
		0x45, 0x00, 0x00, 0x2C,
		0x4C, 0x79, 0x00, 0x00,
		0x40, 0x06, 0x4F, 0x37,
	};
	size_t len = 12;
	
	uint8_t *after = encodeCOBS(before, buf, len);
	uint8_t afterLen = len + 1;

	uint8_t expected[COBS_DECODE_MAX_LENGTH] = {
		0x02, 0x45, 0x01, 0x04,
		0x2C, 0x4C, 0x79, 0x01,
		0x05, 0x40, 0x06, 0x4F,
		0x37,
	};
	
	assert(memcmp(after, expected, afterLen) == 0);
}

void encodeLong(void) {
	uint8_t before[COBS_ENCODE_MAX_LENGTH];
	for (size_t i = 0; i < COBS_ENCODE_MAX_LENGTH; i++) {
		before[i] = 0x01;
	}
	size_t len = COBS_ENCODE_MAX_LENGTH;
	
	uint8_t *after = encodeCOBS(before, buf, len);
	uint8_t afterLen = len + 1;

	uint8_t expected[COBS_DECODE_MAX_LENGTH];
	expected[0] = 0xFF;
	for (size_t i = 1; i < COBS_DECODE_MAX_LENGTH; i++) {
		expected[i] = 0x01;
	}

	assert(memcmp(after, expected, afterLen) == 0);
}

void encodeTooLong(void) {
	size_t len = COBS_ENCODE_MAX_LENGTH + 1;
	uint8_t before[len];
	for (size_t i = 0; i < len; i++) {
		before[i] = 0x01;
	}
	
	uint8_t *after = encodeCOBS(before, buf, len);
	uint8_t afterLen = len + 1;

	uint8_t expected[COBS_DECODE_MAX_LENGTH];
	expected[0] = 0xFF;
	for (size_t i = 1; i < COBS_DECODE_MAX_LENGTH; i++) {
		expected[i] = 0x01;
	}

	assert(memcmp(after, expected, afterLen) == 0);
}

void decodeSimple(void) {
	uint8_t before[COBS_DECODE_MAX_LENGTH] = {
		0x02, 0x45, 0x01, 0x04,
		0x2C, 0x4C, 0x79, 0x01,
		0x05, 0x40, 0x06, 0x4F,
		0x37,
	};
	size_t len = 13;
	
	uint8_t *after = decodeCOBS(before, buf, len);
	uint8_t afterLen = len - 1;

	uint8_t expected[COBS_ENCODE_MAX_LENGTH] = {
		0x45, 0x00, 0x00, 0x2C,
		0x4C, 0x79, 0x00, 0x00,
		0x40, 0x06, 0x4F, 0x37,
	};
	
	assert(memcmp(after, expected, afterLen) == 0);
}

void decodeLong(void) {
	uint8_t before[COBS_DECODE_MAX_LENGTH];
	before[0] = 0xFF;
	for (size_t i = 1; i < COBS_DECODE_MAX_LENGTH; i++) {
		before[i] = 0x01;
	}
	size_t len = COBS_DECODE_MAX_LENGTH;
	
	uint8_t *after = decodeCOBS(before, buf, len);
	uint8_t afterLen = len - 1;

	uint8_t expected[COBS_ENCODE_MAX_LENGTH];
	for (size_t i = 0; i < COBS_ENCODE_MAX_LENGTH; i++) {
		expected[i] = 0x01;
	}

	assert(memcmp(after, expected, afterLen) == 0);
}

void decodeTooLong(void) {
	size_t len = COBS_DECODE_MAX_LENGTH + 1;
	uint8_t before[len];
	before[0] = 0xFF;
	for (size_t i = 1; i < len; i++) {
		before[i] = 0x01;
	}
	
	uint8_t *after = decodeCOBS(before, buf, len);
	uint8_t afterLen = len - 1;

	uint8_t expected[COBS_ENCODE_MAX_LENGTH];
	for (size_t i = 0; i < COBS_ENCODE_MAX_LENGTH; i++) {
		expected[i] = 0x01;
	}

	assert(memcmp(after, expected, afterLen) == 0);
}

void main(void) {
	encodeSimple();
	encodeLong();
	encodeTooLong();
	
	decodeSimple();
	decodeLong();
	decodeTooLong();

	printf("All tests passed!");
}
