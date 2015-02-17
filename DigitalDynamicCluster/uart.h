/*
 * uart.h
 *
 * Created: 2/16/2015 7:05:57 PM
 *  Author: Matt
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL // 16 MHz
#define BAUD 115200

#include <avr/sfr_defs.h>

void uartInit(void);
void uartPutChar(char c);
void uartPutString(char* s);

#endif /* UART_H_ */