/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3

 some of these replies are unused, but it works perfectly as-is so i don't wanna mess with this.
 THE SWITCH WILL NOT RECOGNIZE THE FW AS A CONTROLLER IF THE WRONG VALUES ARE CHANGED, BE CAREFUL.

 */ 

#include "input01PacketReplies.h"
#include "Constants.h"
#include <string.h>
#include <avr/pgmspace.h>
#include <LUFA/Drivers/Peripheral/Serial.h>

#define EXTRA_DATA_START (12)

bool repliedToInitial_01_40_40 = false;

static inline void CopyToInputBuffer(const void* data, uint8_t len)
{
	memcpy_P(inputReply21 + EXTRA_DATA_START, data, len);
}

static inline void ClearInputBuffer(uint8_t len)
{
	memset(inputReply21 + EXTRA_DATA_START, 0, len);
}

#pragma region Packet 01 checks, before sends and replies
//variable naming could use some work tbh
//some of these are unused but i havent gotten around to removing them yet

#pragma region 02 Reply
//All of these checks assume we passed the inputBuffer[0] == 0x01 check
const uint8_t PROGMEM reply_02_Data[] = {
	0x00, 0x82, 0x02, 0x03, 0x48, 0x03, 0x02, 0x04, 0x03, 0xD6, 0x1B, 0x27, 0x6C, 0x03, 0x01
};

bool check_02(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x02;
}

void beforeSend_02()
{
	CopyToInputBuffer(reply_02_Data, sizeof(reply_02_Data));
}

void afterSend_02()
{
	ClearInputBuffer(sizeof(reply_02_Data));
}
#pragma endregion

#pragma region 08 Reply
const uint8_t PROGMEM reply_08_Data[] = {
	0x00, 0x80, 0x08
};

bool check_08(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x08;
}

void beforeSend_08()
{
	CopyToInputBuffer(reply_08_Data, sizeof(reply_08_Data));
}

void afterSend_08()
{
	ClearInputBuffer(sizeof(reply_08_Data));
}
#pragma endregion

#pragma region 10, 00, 60 Reply
const uint8_t PROGMEM reply_10_00_60_Data[] = {
	0x00, 0x90, 0x10, 0x00, 0x60, 0x00, 0x00, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

bool check_10_00_60(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x10 && switchReadBuffer[EXTRA_DATA_INPUT_START + 1] == 0x00 && switchReadBuffer[EXTRA_DATA_INPUT_START + 2] == 0x60;
}

void beforeSend_10_00_60()
{
	CopyToInputBuffer(reply_10_00_60_Data, sizeof(reply_10_00_60_Data));
}

void afterSend_10_00_60()
{
	ClearInputBuffer(sizeof(reply_10_00_60_Data));
}
#pragma endregion

#pragma region 10, 3D Reply
const uint8_t PROGMEM reply_10_3D_Data[] = {
	0x00, 0x90, 0x10, 0x3D, 0x60, 0x00, 0x00, 0x19, 0xAE, 0x55, 0x6F, 0xCF,0x57, 0x75, 0x4A, 0x86, 
	0x59, 0x32,	0x68, 0x76, 0x64, 0xE6, 0x5C, 0xB8, 0xD5, 0x65, 0xFF, 0x32,0x32, 0x32, 0xFF, 0xFF,
	0xFF
};

bool check_10_3D(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x10 && switchReadBuffer[EXTRA_DATA_INPUT_START + 1] == 0x3D;
}

void beforeSend_10_3D()
{
	CopyToInputBuffer(reply_10_3D_Data, sizeof(reply_10_3D_Data));
}

void afterSend_10_3D()
{
	ClearInputBuffer(sizeof(reply_10_3D_Data));
}
#pragma endregion

#pragma region 01, 04, Reply
const uint8_t PROGMEM reply_01_04_Data[] = {
	0x00, 0x81, 0x01, 0x03
};

bool check_01_04(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x01 && switchReadBuffer[EXTRA_DATA_INPUT_START + 1] == 0x04;
}

void beforeSend_01_04()
{
	CopyToInputBuffer(reply_01_04_Data, sizeof(reply_01_04_Data));
}

void afterSend_01_04()
{
	ClearInputBuffer(sizeof(reply_01_04_Data));
}
#pragma endregion

#pragma region 03 Reply //doesn't exist?
const uint8_t PROGMEM reply_03_Data[] = {
	0x00, 0x80, 0x03
};

bool check_03(void)
{
	return switchReadBuffer[EXTRA_DATA_INPUT_START] == 0x03;
}

void beforeSend_03()
{
	CopyToInputBuffer(reply_03_Data, sizeof(reply_03_Data));
}

void afterSend_03()
{
	ClearInputBuffer(sizeof(reply_03_Data));
}
#pragma endregion

#pragma region 04 Reply
const uint8_t PROGMEM reply_04_Data[] = {
	0x00, 0x83, 0x04
};

bool check_04(void)
{
	return switchReadBuffer[10] == 0x04;
}

void beforeSend_04()
{
	CopyToInputBuffer(reply_04_Data, sizeof(reply_04_Data));
}

void afterSend_04()
{
	ClearInputBuffer(sizeof(reply_04_Data));
}
#pragma endregion

#pragma region 10, 80 Reply
const uint8_t PROGMEM reply_10_80_Data[] = {
	0x00, 0x90, 0x10, 0x80, 0x60, 0x00, 0x00, 0x18, 0x50, 0xFD, 0x00, 0x00, 0xC6, 0x0F, 0x0F, 0x30, 
	0x61, 0x96,	0x30, 0xF3, 0xD4, 0x14, 0x54, 0x41, 0x15, 0x54, 0xC7, 0x79, 0x9C, 0x33, 0x36, 0x63
};

bool check_10_80(void)
{
	return switchReadBuffer[10] == 0x10 && switchReadBuffer[11] == 0x80;
}

void beforeSend_10_80()
{
	CopyToInputBuffer(reply_10_80_Data, sizeof(reply_10_80_Data));
}

void afterSend_10_80()
{
	ClearInputBuffer(sizeof(reply_10_80_Data));
}
#pragma endregion

#pragma region 10, 98 Reply
const uint8_t PROGMEM reply_10_98_Data[] = {
	0x00, 0x90, 0x10, 0x98, 0x60, 0x00, 0x00, 0x12, 0x0F, 0x30, 0x61, 0x96, 0x30, 0xF3, 0xD4, 0x14, 
	0x54, 0x41,	0x15, 0x54, 0xC7, 0x79, 0x9C, 0x33, 0x36, 0x63
};

bool check_10_98(void)
{
	return switchReadBuffer[10] == 0x10 && switchReadBuffer[11] == 0x98;
}

void beforeSend_10_98()
{
	CopyToInputBuffer(reply_10_98_Data, sizeof(reply_10_98_Data));
}

void afterSend_10_98()
{
	ClearInputBuffer(sizeof(reply_10_98_Data));
}
#pragma endregion

#pragma region 10, 10 Reply
const uint8_t PROGMEM reply_10_10_Data[] = {
	0x00, 0x90, 0x10, 0x10, 0x80, 0x00, 0x00, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF,	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

bool check_10_10(void)
{
	return switchReadBuffer[10] == 0x10 && switchReadBuffer[11] == 0x10;
}

void beforeSend_10_10()
{
	CopyToInputBuffer(reply_10_10_Data, sizeof(reply_10_10_Data));
}

void afterSend_10_10()
{
	ClearInputBuffer(sizeof(reply_10_10_Data));
}
#pragma endregion

#pragma region 10, 20 Reply
const uint8_t PROGMEM reply_10_20_Data[] = {
	0x0a, 0x90, 0x10, 0x20, 0x60, 0x00, 0x00, 0x18, 0xE6, 0xFF, 0x3A, 0x00, 0x39, 0x00, 0x00, 0x40, 
	0x00, 0x40,	0x00, 0x40, 0xF7, 0xFF, 0xFC, 0xFF, 0x00, 0x00, 0xE7, 0x3B, 0xE7, 0x3B, 0xE7, 0x3B
};

bool check_10_20(void)
{
	return switchReadBuffer[10] == 0x10 && switchReadBuffer[11] == 0x20 && switchReadBuffer[12] == 0x60;
}

void beforeSend_10_20()
{
	CopyToInputBuffer(reply_10_20_Data, sizeof(reply_10_20_Data));
}

void afterSend_10_20()
{
	ClearInputBuffer(sizeof(reply_10_20_Data));
}
#pragma endregion

#pragma region 48, 01 Reply
const uint8_t PROGMEM reply_48_01_Data[] = {
	0x48
};

bool check_48_01(void)
{
	return switchReadBuffer[10] == 0x48 && switchReadBuffer[11] == 0x01;
}

void beforeSend_48_01()
{
	CopyToInputBuffer(reply_48_01_Data, sizeof(reply_48_01_Data));
}

void afterSend_48_01()
{
	ClearInputBuffer(sizeof(reply_48_01_Data));
}
#pragma endregion

#pragma region 40, 01 Reply
const uint8_t PROGMEM reply_40_01_Data[] = {
	0x40
};

bool check_40_01(void)
{
	return switchReadBuffer[10] == 0x40 && switchReadBuffer[11] == 0x01;
}

void beforeSend_40_01()
{
	CopyToInputBuffer(reply_40_01_Data, sizeof(reply_40_01_Data));
}

void afterSend_40_01()
{
	ClearInputBuffer(sizeof(reply_40_01_Data));
}
#pragma endregion

#pragma region 30, 01 Reply
const uint8_t PROGMEM reply_30_01_Data[] = {
	0x30
};

bool check_30_01(void)
{
	return switchReadBuffer[10] == 0x30 && switchReadBuffer[11] == 0x01;
}

void beforeSend_30_01()
{
	CopyToInputBuffer(reply_30_01_Data, sizeof(reply_30_01_Data));
}

void afterSend_30_01()
{
	ClearInputBuffer(sizeof(reply_30_01_Data));
}
#pragma endregion

#pragma region 22, 01 Reply
const uint8_t PROGMEM reply_22_01_Data[] = {
	0x22
};

bool check_22_01(void)
{
	return switchReadBuffer[10] == 0x22 && switchReadBuffer[11] == 0x01;
}

void beforeSend_22_01()
{
	CopyToInputBuffer(reply_22_01_Data, sizeof(reply_22_01_Data));
}

void afterSend_22_01()
{
	ClearInputBuffer(sizeof(reply_22_01_Data));
}
#pragma endregion

#pragma region 01,40,40,00,01,40,40 Reply
const uint8_t PROGMEM reply_01_40_Data[] = {
	0x0a, 0x90, 0x10, 0x3d, 0x60, 0x00, 0x00, 0x19, 0xae, 0x55, 0x6f, 0xcf, 0x57, 0x75, 0x4a, 0x86, 
	0x59, 0x32, 0x68, 0x76, 0x64, 0xe6, 0x5c, 0xb8, 0xd5, 0x65, 0xff, 0x32, 0x32, 0x32, 0xff, 0xff, 
	0xff
};

bool check_01_40(void)
{
	if(repliedToInitial_01_40_40) return false; //we already sent our reply, no need to reply again
	bool isMatch = switchReadBuffer[2] == 0x00 && switchReadBuffer[3] == 0x01 && switchReadBuffer[4] == 0x40 && switchReadBuffer[5] == 0x40;
	if(isMatch) repliedToInitial_01_40_40 = true;
	return isMatch;
}

void beforeSend_01_40()
{
	CopyToInputBuffer(reply_01_40_Data, sizeof(reply_01_40_Data));
}

void afterSend_01_40()
{
	ClearInputBuffer(sizeof(reply_01_40_Data));
}
#pragma endregion

#pragma endregion


Packet01ReplyStruct replyStruct[] = 
{
	REPLY_STRUCT_ITEM(check_02,			beforeSend_02,			afterSend_02),
	REPLY_STRUCT_ITEM(check_08,			beforeSend_08,			afterSend_08),
	REPLY_STRUCT_ITEM(check_10_00_60,	beforeSend_10_00_60,	afterSend_10_00_60),
	REPLY_STRUCT_ITEM(check_10_3D,		beforeSend_10_3D,		afterSend_10_3D),
	REPLY_STRUCT_ITEM(check_01_04,		beforeSend_01_04,		afterSend_01_04),
	REPLY_STRUCT_ITEM(check_03,			beforeSend_03,			afterSend_03),
	REPLY_STRUCT_ITEM(check_04,			beforeSend_04,			afterSend_04),
	REPLY_STRUCT_ITEM(check_10_80,		beforeSend_10_80,		afterSend_10_80),
	REPLY_STRUCT_ITEM(check_10_98,		beforeSend_10_98,		afterSend_10_98),
	REPLY_STRUCT_ITEM(check_10_10,		beforeSend_10_10,		afterSend_10_10),
	REPLY_STRUCT_ITEM(check_10_20,		beforeSend_10_20,		afterSend_10_20),
	REPLY_STRUCT_ITEM(check_48_01,		beforeSend_48_01,		afterSend_48_01),
	REPLY_STRUCT_ITEM(check_40_01,		beforeSend_40_01,		afterSend_40_01),
	REPLY_STRUCT_ITEM(check_30_01,		beforeSend_30_01,		afterSend_30_01),
	REPLY_STRUCT_ITEM(check_22_01,		beforeSend_22_01,		afterSend_22_01),
	REPLY_STRUCT_END
};

//publically visible methods below

void ExecuteBeforeSend(uint8_t offset)
{
	replyStruct[offset].BeforeSend();
}

void ExecuteAfterSend(uint8_t offset)
{
	replyStruct[offset].AfterSend();
}

bool ShouldSendFullInput(uint8_t offset)
{
	switch(offset)
	{
		case 0:
		case 1:
		{
			return true;
		}

		default: return false;
	}
}

bool ShouldEnableFullInputLoop(uint8_t offset)
{
	switch(offset)
	{
		case 10:
		{
			return true;
		}

		default: return false;
	}
}

uint8_t CheckIfReplyTo01Packet(void)
{
	uint8_t i = 0;
	while(replyStruct[i].CheckPacket != 0)
	{
		if(replyStruct[i].CheckPacket()) return i;
		i++;
	}

	return NO_SUITABLE_RESPONSE;
}

void ResetReplyData(void)
{
	repliedToInitial_01_40_40 = false;
}