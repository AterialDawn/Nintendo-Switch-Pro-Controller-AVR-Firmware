/*
 * Nintendo Switch Pro Controller emulator firmware for AVR devices
 * Copyright (C) 2017 AterialDawn (aterial@aterialdawn.com)
 * License: GPLv3
 */


#ifndef CONSTANTS_H_
#define CONSTANTS_H_


//Handy LED control
#define TX_RX_LED_INIT  (DDRD |= (1<<5), DDRB |= (1<<0))
#define TXLED0          (PORTD |= (1<<5))
#define TXLED1          (PORTD &= ~(1<<5))
#define RXLED0          (PORTB |= (1<<0))
#define RXLED1          (PORTB &= ~(1<<0))

#define EP_IN_ADDR	(0x81)
#define EP_OUT_ADDR	(0x02)
#define EP_SIZE 64

extern uint8_t switchReadBuffer[EP_SIZE];

extern uint8_t inputReply[EP_SIZE];
extern uint8_t inputReply21[EP_SIZE];


#endif /* CONSTANTS_H_ */