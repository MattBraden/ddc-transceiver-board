/*
 * DigitalDynamicCluster.c
 *
 * Created: 2/5/2015 4:02:15 PM
 *  Author: Matt
 */ 

#define F_CPU 16000000UL // 16 MHz
#define BAUD 115200

#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
//#include "uart.h"

void uart_init(void) {
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

void uart_putchar(char c) {
	loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
	UDR0 = c;
}

void uart_putstring(char* s) {
	while (*s)
		uart_putchar(*s++);
		
}

int main(void)
{
	//uart_init(UART_BAUD_SELECT(9600, F_CPU));
	uart_init();
	DDRA = 0xFF;
	int count = 0;
	char buffer[20];
	while(1)
    {
		 //_delay_ms(500);
		 itoa(count++, buffer, 10);
		 uart_putstring(buffer);
		 uart_putstring("\n");
		 PORTA = ~PORTA;
    }
}