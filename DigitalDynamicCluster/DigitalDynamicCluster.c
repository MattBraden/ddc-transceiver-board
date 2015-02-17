/*
 * DigitalDynamicCluster.c
 *
 * Created: 2/5/2015 4:02:15 PM
 *  Author: Matt
 */ 

// TODO: Define cpu and baud here instead of in the uart.h
#define F_CPU 16000000UL // 16 MHz
//#define BAUD 115200

#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "uart.h"
#include "MCP2515_defs.h"

#define WIFI_IDLE PORTB |= _BV(PORTB3)
#define WIFI_ACTIVE PORTB &= ~_BV(PORTB3)
#define MCP2515_IDLE PORTB |= _BV(PORTB4)
#define MCP2515_ACTIVE PORTB &= ~_BV(PORTB4)

void CANWrite(uint8_t addr, uint8_t data)
{
	MCP2515_ACTIVE;
	spiTransceiver(CAN_WRITE);
	spiTransceiver(addr);
	spiTransceiver(data);
	MCP2515_IDLE;
}

uint8_t CANRead(uint8_t addr)
{
	uint8_t data;
	MCP2515_ACTIVE;
	spiTransceiver(CAN_READ);
	spiTransceiver(addr);
	data = spiTransceiver(0x00);
	MCP2515_IDLE;
	return data;
}

void mcp2515Init(void) {
	 CANWrite(CANCTRL, 0b10000111); // conf mode
	 CANWrite(CNF1, 0x00); // CNF1 b00000000
	 CANWrite(CNF2,	0xA4); // CNF2 b10100100
	 CANWrite(CNF3,	0x84); // CNF3 b10000100
	 CANWrite(CANCTRL, 0b00000111); // normal mode
	 _delay_ms(10);
	 CANWrite(RXB0CTRL, 0b01101000); //RXB0CTRL clear receive buffers
	 CANWrite(RXB1CTRL,	0b01101000); //RXB1CTRL clear receive buffers
	 CANWrite(CANINTE, 0b00000011); // enable interrupt on RXB0, RXB1
	 CANWrite(BFPCTRL, 0b00001111); // setting interrupts
}


ISR(SPI_STC_vect) {
	uartPutString("Interrupt\n");
	// Wait to receive data from slave
	//uint8_t data = spiReceive();
	// Do something with received data
}

int main(void) {
	spiInit();
	uartInit();
	mcp2515Init();
	//sei();
	uint8_t count = 0;
	char buffer[5];
	while(1) {
		 itoa(count++, buffer, 10);
		 uartPutString(buffer);
		 uartPutString("\n");
		 spiTransceiver(count);
		 _delay_ms(500);
    }
}