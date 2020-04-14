#ifndef RELIABLE_SERIAL_H
#define RELIABLE_SERIAL_H

#include <stdint.h>

enum {
	SERIAL_PACKET_MAX_LENGTH = 255,
};

void serialSendPacket(uint8_t *, uint8_t);
bool serialRecvPacket(uint8_t *, uint8_t *);

#endif
