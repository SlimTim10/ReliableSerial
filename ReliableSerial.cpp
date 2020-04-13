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

uint8_t serialRecvPacket(uint8_t *packet) {
	if (Serial.available() == 0) return 0;
	
	while (Serial.available() > 0) {
		uint8_t b = Serial.read();
		
		if (b != COBS_BOUNDARY) {
			packet[0] = b;
			break;
		}
	}

	uint8_t i;
	for (i = 1; Serial.available() > 0 && i < SERIAL_PACKET_MAX_LENGTH; i++) {
		uint8_t b = Serial.read();
		
		if (b == COBS_BOUNDARY) break;
		
		packet[i] = b;
	}

	return i;
}
