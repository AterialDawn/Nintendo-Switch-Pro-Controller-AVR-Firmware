/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

// Includes
#include <LUFA/Drivers/USB/USB.h>

#include <avr/pgmspace.h>

// Device String Descriptor IDs
enum StringDescriptors_t
{
	STRING_ID_Language     = 0, // Supported Languages string descriptor ID (must be zero)
	STRING_ID_Manufacturer = 1, // Manufacturer string ID
	STRING_ID_Product      = 2, // Product string ID
	STRING_ID_SerialNumber = 3,
};


// Descriptor Header Type - HID Class HID Descriptor
#define DTYPE_HID                 0x21
// Descriptor Header Type - HID Class HID Report Descriptor
#define DTYPE_Report              0x22

// Function Prototypes
uint16_t CALLBACK_USB_GetDescriptor(
	const uint16_t wValue,
	const uint16_t wIndex,
	const void** const DescriptorAddress
) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif