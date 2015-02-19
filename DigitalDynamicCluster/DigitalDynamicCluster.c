/*
 * DigitalDynamicCluster.c
 *
 * Created: 2/5/2015 4:02:15 PM
 *  Author: Matt
 */ 

// TODO: Define cpu and baud here instead of in the uart.h
#define F_CPU 16000000UL // 16 MHz
//#define BAUD 115200
// Warren test
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
	MCP2515_ACTIVE;
	_delay_ms(10);
	MCP2515_IDLE;
	_delay_ms(10);
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

void MSrequest(uint8_t block, uint16_t offset, uint8_t req_bytes)
{
	uint8_t SIDH, SIDL, EID8, EID0, DLC, D0, D1, D2;
	
	// Higher 8 bits of address
	// var_offset<7:0>
	SIDH = offset >> 3;

	// Lower 3 bits of address, have to set IDE
	// var_offset<7:5> SRR<4> IDE<3> msg_type<3:0>
	SIDL = ((offset << 5) | 0b0001000);
	
	// From 
	// FromID<7:4> ToID<3:0>
	EID8 = 0b10011000; //:7 msg_req, from id 3 (4:3)
	
	//      TBBBBBSS To, Block, Spare
	EID0 = ( ( block & 0b00001111) << 3); // last 4 bits, move them to 6:3
	EID0 = ((( block & 0b00010000) >> 2) | EID0); // bit 5 goes to :2
	
	DLC = 0b00000011;
	D0=(block);
	D1=(offset >> 3);
	D2=(((offset & 0b00000111) << 5) | req_bytes); // shift offset
	
	
	MCP2515_ACTIVE;
	spiTransceiver(0x40);	// Push bits starting at 0x31 (RXB0SIDH)
	spiTransceiver(SIDH);	//0x31
	spiTransceiver(SIDL);	//0x32
	spiTransceiver(EID8);	//0x33
	spiTransceiver(EID0);	//0x34
	spiTransceiver(DLC);	//0x35
	spiTransceiver(D0);		// 0x36 TXB0D0 my_varblk
	spiTransceiver(D1);		// 0x37 TXB0D1 my_offset
	spiTransceiver(D2);		// 0x38 TXB0D2 - request 8 bytes(?) from MS3
	MCP2515_IDLE;			// end write
	
	// RTS - Send this buffer down the wire
	MCP2515_ACTIVE;
	spiTransceiver(0b10000001);
	MCP2515_IDLE;
	
	CANWrite(CANINTF,0x00);
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
	MSrequest(7, 6, 2);
	
	while(1) {
		 itoa(count++, buffer, 10);
		 uartPutString(buffer);
		 uartPutString("\n");
		 spiTransceiver(count);
		 _delay_ms(500);
    }
}