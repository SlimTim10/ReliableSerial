#ifndef ENCODING_H
#define ENCODING_H

#include <stdint.h>
#include <stddef.h>

enum EncodingSettings {
	COBS_ENCODE_MAX_LENGTH = 254,
	COBS_DECODE_MAX_LENGTH = 255,
	COBS_BOUNDARY = 0x00,
};

/*
Byte stuffs "length" bytes of data at the location pointed to by "src", writing the output to the location pointed to by "dst". Returns a pointer to "dst".

Source:
S. Cheshire and M. Baker, "Consistent Overhead Byte Stuffing," in ACM SIGCOMM '97, Cannes, France.
 */
#define FinishBlock(X) (*codePtr = (X), codePtr = dst++, code = 0x01)
uint8_t *cobsEncode(uint8_t *src, uint8_t *dst, size_t length) {
	if (length > COBS_ENCODE_MAX_LENGTH) {
		length = COBS_ENCODE_MAX_LENGTH;
	}

	uint8_t *dstPtr = dst;

	src[length++] = 0x00;
	
	uint8_t *end = src + length;
	uint8_t *codePtr = dst++;
	uint8_t code = 0x01;
	while (src < end) {
		if (*src == 0) FinishBlock(code);
		else {
			*dst++ = *src;
			code++;
			if (code == 0xFF) FinishBlock(code);
		}
		src++;
	}
	FinishBlock(code);

	return dstPtr;
}

/*
Decodes "length" bytes of data at the location pointed to by "src", writing the output to the location pointed to by "dst".

Source:
S. Cheshire and M. Baker, "Consistent Overhead Byte Stuffing," in ACM SIGCOMM '97, Cannes, France.
 */
uint8_t *cobsDecode(uint8_t *src, uint8_t *dst, size_t length) {
	if (length > COBS_DECODE_MAX_LENGTH) {
		length = COBS_DECODE_MAX_LENGTH;
	}

	uint8_t *dstPtr = dst;
	
	uint8_t *end = src + length;

	while (src < end) {
		uint8_t code = *src++;
		size_t i;
		for (i = 1; i < code; i++) *dst++ = *src++;
		if (code < 0xFF) *dst++ = 0;
	}

	return dstPtr;
}

#endif
