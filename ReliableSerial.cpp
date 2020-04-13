#include <Arduino.h>
#include <functional.h>

#include "util/encoding.h"
#include "util/checksum.h"

void serialSendPacket(uint8_t *packet, size_t length) {
	uint8_t buf[COBS_ENCODE_MAX_LENGTH];
	uint8_t *encPacket = cobsEncode(packet, buf, length);
	uint8_t encLen = length + 1;
	Serial.write(COBS_BOUNDARY);
	forEach(encPacket, encLen, Serial.write);
	Serial.write(COBS_BOUNDARY);
	Serial.flush();
}
