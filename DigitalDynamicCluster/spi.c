/*
 * spi.c
 *
 * Created: 2/16/2015 7:06:15 PM
 *  Author: Matt
 */ 

#include "spi.h"
#include <avr/io.h>

void spiInit(void) {
	// Set MOSI, SS, and SCK output, all others input
	DDRB = (1<<SS_BIT)|(1<<MOSI_BIT)|(1<<SCK_BIT);
	// Enable SPI, Master, set clock rate fck/4
	SPCR0 = /*(1<<SPIE0)|*/(1<<SPE0)|(1<<MSTR0);
}

void spiTransmit(uint8_t data) {
	// Start transmission
	SPDR0 = data;
	// Wait for transmission complete
	while(!(SPSR0 & (1<<SPIF0)));
	// Clear flag
	//SPSR0 &= ~(1<<SPIF0);
}

uint8_t spiReceive(void) {
	// Wait for reception complete
	while(!(SPSR0 & (1<<SPIF0)));
	// Return data register
	return SPDR0;
}

uint8_t spiTransceiver(uint8_t data) {
	// Load data into the buffer
	SPDR0 = data;
	
	//Wait until transmission complete
	while(!(SPSR0 & (1<<SPIF0)));
	
	// Return received data
	return(SPDR0);
}
