/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */


#include "serialProto.h"
#include "Constants.h"
#include <avr/wdt.h>

//Offsets into the input packets, and length of expected values
#define VAR_BUTTON_START (2)
#define VAR_BUTTON_LEN (4)

#define VAR_STICKS_L_START (6)
#define VAR_STICKS_R_START (9)
#define VAR_STICKS_LEN (3)

#define VAR_ACCELEROMETER_START (13)
#define VAR_GYROSCOPE_START (19)
#define VAR_ACCEL_GYRO_LEN (6)


#define FAST_INPUT_OFFSET (2)
#define FAST_INPUT_LEN (10)

#define GYRO_INPUT_OFFSET			(19)
#define GYRO_INPUT_OFFSET_DELTA1	(31)
#define GYRO_INPUT_OFFSET_DELTA2	(43)
#define GYROACCEL_INPUT_LEN			(6)

#define ACCEL_INPUT_OFFSET			(13)
#define ACCEL_INPUT_OFFSET_DELTA1	(25)
#define ACCEL_INPUT_OFFSET_DELTA2	(37)

#define FULL_INPUT_OFFSET (2)
#define FULL_INPUT_LEN (47)

bool readyForInput = true;

static inline void InputRead()
{
	readyForInput = false;
}

//borrowed from Matlo's serialusb project
static inline void forceHardReset(void)
{
	TXLED1;
	RXLED1;

	cli(); // disable interrupts
	wdt_enable(WDTO_15MS); // enable watchdog
	while(1); // wait for watchdog to reset processor
}

//various functions for reading packets from serial and writing them to corresponding report arrays

static inline void ReadFastInputPacket(void)
{
	uint8_t b = 0;
	for(uint8_t i = 0; i < FAST_INPUT_LEN; i++)
	{
		b = Serial_BlockingReceiveByte();

		inputReply[FAST_INPUT_OFFSET + i] = b;
		inputReply21[FAST_INPUT_OFFSET + i] = b;
	}

	InputRead();
}

static inline void ReadGyroInputPacket(void)
{
	uint8_t b = 0;
	for(uint8_t i = 0; i < GYROACCEL_INPUT_LEN; i++)
	{
		b = Serial_BlockingReceiveByte();

		inputReply[		GYRO_INPUT_OFFSET			+ i]	= b;
		inputReply[		GYRO_INPUT_OFFSET_DELTA1	+ i]	= b;
		inputReply[		GYRO_INPUT_OFFSET_DELTA2	+ i]	= b;
		inputReply21[	GYRO_INPUT_OFFSET			+ i]	= b;
		inputReply21[	GYRO_INPUT_OFFSET_DELTA1	+ i]	= b;
		inputReply21[	GYRO_INPUT_OFFSET_DELTA2	+ i]	= b;
	}

	InputRead();
}

static inline void ReadAccelInputPacket(void)
{
	uint8_t b = 0;
	for(uint8_t i = 0; i < GYROACCEL_INPUT_LEN; i++)
	{
		b = Serial_BlockingReceiveByte();

		inputReply[ACCEL_INPUT_OFFSET + i] = b;
		inputReply21[ACCEL_INPUT_OFFSET + i] = b;
	}

	InputRead();
}

static inline void ReadFullInputPacket(void)
{
	uint8_t b = 0;
	for(uint8_t i = 0; i < FULL_INPUT_LEN; i++)
	{
		b = Serial_BlockingReceiveByte();

		inputReply[FULL_INPUT_OFFSET + i] = b;
		inputReply21[FULL_INPUT_OFFSET + i] = b;
	}

	InputRead();
}

/*
	Variable input packets must be laid out in a specific way. Assuming all VariableUpdateTypes are present, the packet shall contain data as such
	-----------------
	Buttons_4b
	LeftStick_3b
	RightStick_3b
	Accelerometer_6b
	Gyroscope_6b
	-----------------
*/
static inline void ReadVariableInputPacket(void)
{
	uint8_t updateType = Serial_BlockingReceiveByte();
	uint8_t b;

	//theres probably a better way to do this
	if((updateType & UPD_BUTTONS) == UPD_BUTTONS)
	{
		for(uint8_t i = 0; i < VAR_BUTTON_LEN; i++)
		{
			b = Serial_BlockingReceiveByte();

			inputReply	[VAR_BUTTON_START + i] = b;
			inputReply21[VAR_BUTTON_START + i] = b;
		}
	}

	if((updateType & UPD_STICK_LEFT) == UPD_STICK_LEFT)
	{
		for(uint8_t i = 0; i < VAR_STICKS_LEN; i++)
		{
			b = Serial_BlockingReceiveByte();

			inputReply	[VAR_STICKS_L_START + i] = b;
			inputReply21[VAR_STICKS_L_START + i] = b;
		}
	}

	if((updateType & UPD_STICK_RIGHT) == UPD_STICK_RIGHT)
	{
		for(uint8_t i = 0; i < VAR_STICKS_LEN; i++)
		{
			b = Serial_BlockingReceiveByte();

			inputReply	[VAR_STICKS_R_START + i] = b;
			inputReply21[VAR_STICKS_R_START + i] = b;
		}
	}
	//In the next 2 updates, we copy the immediate gyro/accel values into the memory regions where the 5ms ago, and 10ms ago values are supposed to be to avoid sending 3x as much data.

	if((updateType & UPD_ACCELEROMETER) == UPD_ACCELEROMETER)
	{
		for(uint8_t i = 0; i < VAR_ACCEL_GYRO_LEN; i++)
		{
			b = Serial_BlockingReceiveByte();

			inputReply	[VAR_ACCELEROMETER_START + i]	= b;
			inputReply	[ACCEL_INPUT_OFFSET_DELTA1 + i] = b;
			inputReply	[ACCEL_INPUT_OFFSET_DELTA2 + i] = b;

			inputReply21[VAR_ACCELEROMETER_START + i]	= b;
			inputReply21[ACCEL_INPUT_OFFSET_DELTA1 + i] = b;
			inputReply21[ACCEL_INPUT_OFFSET_DELTA2 + i] = b;
		}
	}

	if((updateType & UPD_GYROSCOPE) == UPD_GYROSCOPE)
	{
		for(uint8_t i = 0; i < VAR_ACCEL_GYRO_LEN; i++)
		{
			b = Serial_BlockingReceiveByte();

			inputReply	[VAR_GYROSCOPE_START + i]		= b;
			inputReply	[GYRO_INPUT_OFFSET_DELTA1 + i]	= b;
			inputReply	[GYRO_INPUT_OFFSET_DELTA2 + i]	= b;

			inputReply21[VAR_GYROSCOPE_START + i]		= b;
			inputReply21[GYRO_INPUT_OFFSET_DELTA1 + i]	= b;
			inputReply21[GYRO_INPUT_OFFSET_DELTA2 + i]	= b;
		}
	}
	InputRead();
}

ISR(USART1_RX_vect) {
	uint8_t packetType = UDR1;
	switch(packetType)
	{
		case P_RESET:
		{
			forceHardReset();
			break; //we'll die before we reach this but hey i like consistency
		}
		case P_FAST_INPUT:
		{
			//Fast inputs are always 10 bytes long
			ReadFastInputPacket();
			break;
		}
		case P_GYRO_INPUT:
		{
			//Gyro inputs are always 6 bytes long
			ReadGyroInputPacket();
			break;
		}
		case P_ACCEL_INPUT:
		{
			//Accelerometer inputs are always 6 bytes long
			ReadAccelInputPacket();
			break;
		}
		case P_VARIABLE_INPUT:
		{
			//Variable input packets are variable...
			ReadVariableInputPacket();
			break;
		}
		case P_FULL_INPUT:
		{
			//Full inputs are always 47 bytes long due to gyro/accel seemingly being time averaged
			ReadFullInputPacket();
			break;
		}
		case P_CAN_SEND_UPDATE:
		{
			//simple reply
			readyForInput ? Serial_SendByte(P_DO_SEND_UPDATE) : Serial_SendByte(P_DONT_SEND_UPDATE);
		}
	}
}

void SP_InputSent()
{
	bool oldInput = readyForInput;
	readyForInput = true;
	if(oldInput != readyForInput) Serial_SendByte(P_DO_SEND_UPDATE);
}