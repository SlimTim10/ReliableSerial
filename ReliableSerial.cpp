#include <Arduino.h>
#include <functional.h>

#include "util/encoding.h"
#include "util/checksum.h"

#include "ReliableSerial.h"

void serialSendPacket(uint8_t *packet, uint8_t length) {
	uint8_t *next = withCheckBytes(packet, packet, length);
	uint8_t nextLength = length + 2;
	
	uint8_t buf[SERIAL_PACKET_MAX_LENGTH];
	uint8_t *encPacket = cobsEncode(next, buf, nextLength);
	uint8_t encLen = nextLength + 1;
	
	Serial.write(COBS_BOUNDARY);
	forEach(encPacket, encLen, Serial.write);
	Serial.write(COBS_BOUNDARY);
	Serial.flush();
}

// Packets are surrounded by null bytes, have two check bytes at the end, and are COBS-encoded
// Returns true if a valid packet is received, otherwise false
bool serialRecvPacket(uint8_t *packet, uint8_t *length) {
	// No data available
	if (Serial.available() == 0) return false;

	uint8_t buf[SERIAL_PACKET_MAX_LENGTH];

	// Skip until reaching packet data
	while (Serial.available() > 0) {
		uint8_t b = Serial.read();
		
		if (b != COBS_BOUNDARY) {
			buf[0] = b;
			break;
		}
	}

	// Receive all packet data
	uint8_t i;
	for (i = 1; Serial.available() > 0 && i < SERIAL_PACKET_MAX_LENGTH; i++) {
		uint8_t b = Serial.read();
		
		if (b == COBS_BOUNDARY) break;
		
		buf[i] = b;
	}

	// COBS decode
	uint8_t *decPacket = cobsDecode(buf, packet, i);
	uint8_t decLen = i - 1;

	// Checksum
	if (check(decPacket, decLen) == false) {
		return false;
	}

	packet = withoutCheckBytes(decPacket, packet, decLen);

	*length = decLen - 2;

	return true;
}
