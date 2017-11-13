/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */


#ifndef SERIALPROTO_H_
#define SERIALPROTO_H_

#include <stdint.h>
#include <LUFA/Drivers/Peripheral/Serial.h>

static inline int8_t Serial_BlockingReceiveByte(void)
{
	while(!Serial_IsCharReceived()) {}
	return UDR1;
}

typedef enum {
	P_RESET = 0, //reset the avr
	P_FAST_INPUT = 1, //set buttons/stick axes data
	P_GYRO_INPUT = 2, //set gyro data
	P_ACCEL_INPUT= 3, //set accel data
	P_FULL_INPUT = 4, //set buttons, stick, gyro and accel data
	P_VARIABLE_INPUT = 5, //Followed by a bitmask of what fields to update
	P_CAN_SEND_UPDATE = 6, //asks us whether should receive serial input data
	P_DONT_SEND_UPDATE = 252, //dont send us input data
	P_DO_SEND_UPDATE = 253, //do send us input data
	P_BOOTED = 254, //sent by AVR to indicate we have initialized serial comms
	P_READY = 255 //sent by AVR to indicate the switch has finished recognizing us, and we can send input data
} PacketTypes;

typedef enum {
	UPD_BUTTONS = (1 << 0),
	UPD_STICK_LEFT = (1 << 1),
	UPD_STICK_RIGHT = (1 << 2),
	UPD_ACCELEROMETER = (1 << 3),
	UPD_GYROSCOPE = (1 << 4)
} VariableUpdateTypes;

extern void SP_InputSent();

#endif /* SERIALPROTO_H_ */