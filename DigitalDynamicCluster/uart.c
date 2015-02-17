/*
 * uart.c
 *
 * Created: 2/16/2015 7:06:27 PM
 *  Author: Matt
 */ 

#include "uart.h"
#include <util/setbaud.h>
#include <stdlib.h>
#include <avr/io.h>

void uartInit(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	# if USE_2X
	UCSR0A |= _BV(U2X0);
	#else
	UCSR0A &= ~(_BV(U2X0));
	#endif
	
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uartPutChar(char c) {
	loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
	UDR0 = c;
}

void uartPutString(char* s) {
	while (*s)
	uartPutChar(*s++);
}