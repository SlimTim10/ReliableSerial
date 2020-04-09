#ifndef ENCODING_H
#define ENCODING_H

enum EncodingSettings {
	COBS_ENCODE_MAX_LENGTH = 254,
	COBS_DECODE_MAX_LENGTH = 255,
};

uint8_t *encodeCOBS(uint8_t *, uint8_t *, size_t);
uint8_t *decodeCOBS(uint8_t *, uint8_t *, size_t);

#endif
