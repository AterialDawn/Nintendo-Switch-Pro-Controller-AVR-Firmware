/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */


#ifndef INPUT01PACKETREPLIES_H_
#define INPUT01PACKETREPLIES_H_

#define REPLY_STRUCT_ITEM(CHECK, BEFORE, AFTER) { .CheckPacket = CHECK, .BeforeSend = BEFORE, .AfterSend = AFTER }
#define REPLY_STRUCT_END { .CheckPacket = 0, .BeforeSend = 0, .AfterSend = 0}

#include <stdbool.h>
#include <stdint.h>
#define NO_SUITABLE_RESPONSE (0xff)

#define EXTRA_DATA_INPUT_START (10)

typedef struct Packet01ReplyStruct {
	bool (*CheckPacket)();
	void (*BeforeSend)();
	void (*AfterSend)();
} Packet01ReplyStruct;

extern uint8_t CheckIfReplyTo01Packet(void);
extern void ExecuteBeforeSend(uint8_t offset);
extern void ExecuteAfterSend(uint8_t offset);
extern void ResetReplyData(void);
extern bool ShouldSendFullInput(uint8_t offset);
extern bool ShouldEnableFullInputLoop(uint8_t offset);


#endif /* INPUT01PACKETREPLIES_H_ */