/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */

#ifndef _EMU_H_
#define _EMU_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <LUFA/Version.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/LEDs.h>

#define SERIAL_BAUD_RATE 500000

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
bool EVENT_USB_Device_UnhandledControlRequest(void);
static inline void ReadOutEndpoint(void);

#endif
