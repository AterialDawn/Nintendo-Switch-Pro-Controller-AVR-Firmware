/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */
 
#include "emu.h"
#include "Constants.h"
#include "input01PacketReplies.h"
#include "serialProto.h"

#define COUNTER_INCREMENT 3

#include <LUFA/Drivers/Peripheral/Serial.h>

uint8_t switchReadBuffer[EP_SIZE];

uint8_t packetCounter = 0xaa;
bool sendInputReports = false;
bool txLed = false;
bool rxLed = false;

static uint8_t reply_8101[] = {
	0x81, 0x01, 0x00, 0x03, 0x6c, 0x27, 0x1b, 0xd6, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t reply_8102[] = {
	0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t inputReply[EP_SIZE] = {
	0x30, 0x00, 0x91, 0x00, 0x80, 0x00, 0x19, 0x68, 0x73, 0x3e, 0xa8, 0x73, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//not static since it's referenced in input21PacketReplies.c
uint8_t inputReply21[EP_SIZE] = {
	0x21, 0x00, 0x91, 0x00, 0x80, 0x00, 0x19, 0x68, 0x73, 0x3e, 0xa8, 0x73, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void SetupHardware(void) {

    /* Disable watchdog */
    MCUSR = 0;
    wdt_disable();

    clock_prescale_set(clock_div_1);

    Serial_Init(SERIAL_BAUD_RATE, true);
	UCSR1B |= (1 << RXCIE1); // Enable the USART Receive Complete interrupt (USART_RXC)
	Serial_SendByte(P_BOOTED);

    TX_RX_LED_INIT;
	RXLED0;
	TXLED0;

    USB_Init();
	GlobalInterruptEnable();
}

void EVENT_USB_Device_Connect(void) {

}

void EVENT_USB_Device_Disconnect(void) {

}

void EVENT_USB_Device_ConfigurationChanged(void) {
	ResetReplyData();
	sendInputReports = false;
	bool Success = true;
    Success &= Endpoint_ConfigureEndpoint(EP_IN_ADDR, EP_TYPE_INTERRUPT, EP_SIZE, 1);
	Success &= Endpoint_ConfigureEndpoint(EP_OUT_ADDR, EP_TYPE_INTERRUPT, EP_SIZE, 1);
	if(!Success)
	{
		RXLED0;
		TXLED0;
		for(; ;) ; //die
	}
	RXLED1;
}

bool EVENT_USB_Device_ControlRequest(void) {

    return false;
}

bool EVENT_USB_Device_UnhandledControlRequest(void) {
	switch(USB_ControlRequest.bRequest)
	{
		case HID_REQ_SetIdle: //Difference between old firmware and this #1 : Old FW Sends a Broken Pipe status.
		{
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
				return true;
			}
			break;
		}
	}
    return false;
}

static inline void ReadOutEndpoint(void)
{
	memset(switchReadBuffer, 0, sizeof(switchReadBuffer)); //Clear input buffer before every read

	uint8_t* ReportData = switchReadBuffer;
	uint16_t ReportSize = 0;
	while ( Endpoint_IsReadWriteAllowed())
	{
		uint8_t b = Endpoint_Read_8();
		if( ReportSize < sizeof(switchReadBuffer))
		{
			// avoid over filling of the buffer
			*ReportData = b;
			ReportSize++;
			ReportData++;
		}
	}
	Endpoint_ClearOUT();
}

void SendNextInput(bool force) {
	if(!sendInputReports && !force) return; //don't send input packets if the switch hasn't allowed us to

	Endpoint_SelectEndpoint(EP_IN_ADDR);
	if (Endpoint_IsINReady())
	{
		packetCounter += COUNTER_INCREMENT; //increment counter by predefined amount
		inputReply[1] = packetCounter;
		while(Endpoint_Write_Stream_LE(inputReply, sizeof(inputReply), NULL) != ENDPOINT_RWSTREAM_NoError); //both reports are 64bytes long
		Endpoint_ClearIN();
		SP_InputSent(); //notify serialProto that we have sent the results of the last input report
		(rxLed = !rxLed) ? RXLED1 : RXLED0;
	}
}

void ReceiveNextOutput(void) 
{
	Endpoint_SelectEndpoint(EP_OUT_ADDR);
	if (Endpoint_IsOUTReceived()) 
	{
		(txLed = !txLed) ? TXLED1 : TXLED0;

		ReadOutEndpoint();

		if(switchReadBuffer[0] == 0x80 && switchReadBuffer[1] == 0x01) //Some kind of id?
		{
			//Reply with reply_8001
			Endpoint_SelectEndpoint(EP_IN_ADDR);	//Select IN EP
			while(!Endpoint_IsINReady());					//wait till EP ready
			while(Endpoint_Write_Stream_LE(&reply_8101, sizeof(reply_8101), NULL) != ENDPOINT_RWSTREAM_NoError); //Write reply
			Endpoint_ClearIN();
		}
		else if(switchReadBuffer[0] == 0x80 && switchReadBuffer[1] == 0x02)
		{
			Endpoint_SelectEndpoint(EP_IN_ADDR);	//Select IN EP
			while(!Endpoint_IsINReady());					//wait till EP ready
			while(Endpoint_Write_Stream_LE(&reply_8102, sizeof(reply_8102), NULL) != ENDPOINT_RWSTREAM_NoError); //Write reply
			Endpoint_ClearIN();
		}
		else if(switchReadBuffer[0] == 0x01) //these packets might have a specific handler in input01PacketReplies.c, be very careful when changing the following code.
		{
			uint8_t result = CheckIfReplyTo01Packet();
			if(result != NO_SUITABLE_RESPONSE)
			{
				if(ShouldSendFullInput(result))
				{
					SendNextInput(true);
				}
				else if(ShouldEnableFullInputLoop(result))
				{
					sendInputReports = true;
					Serial_SendByte(P_READY);
					SendNextInput(true);
				}

				ExecuteBeforeSend(result);

				packetCounter += COUNTER_INCREMENT;
				inputReply21[1] = packetCounter;

				Endpoint_SelectEndpoint(EP_IN_ADDR);	//Select IN EP
				while(!Endpoint_IsINReady());					//wait till EP ready
				while(Endpoint_Write_Stream_LE(inputReply21, sizeof(inputReply21), NULL) != ENDPOINT_RWSTREAM_NoError); //both reports are 64bytes long
				Endpoint_ClearIN();

				ExecuteAfterSend(result);
			}
		}
	}
}

void ENDPOINT_Task(void) {

    if (USB_DeviceState != DEVICE_STATE_Configured) {
        return;
    }

    SendNextInput(false);

    ReceiveNextOutput();
}

int main(void) {

    SetupHardware();

    for (;;) {
        ENDPOINT_Task();
        USB_USBTask();
    }
}
