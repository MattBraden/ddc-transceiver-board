/*
 * spi.h
 *
 * Created: 2/16/2015 7:05:43 PM
 *  Author: Matt
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/sfr_defs.h>

void spiInit(void);
void spiTransmit(uint8_t data);
uint8_t spiReceive(void);

uint8_t spiTransceiver(uint8_t data);

#endif /* SPI_H_ */