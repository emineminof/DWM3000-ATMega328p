/*
 * UART.c
 *
 * Created: 9/10/2021 12:32:14 PM
 *  Author: Emin Eminof
 */ 

#include "UART.h"

FILE uart_str = FDEV_SETUP_STREAM(UART_putc, NULL, _FDEV_SETUP_RW);

void uart_init(void)
{
	UBRR0H = UBRRH_VALUE; // these two values are from the setbaud.h macro library
	UBRR0L = UBRRL_VALUE; //

	/* We do not need double speed UART as far as I am aware
	#if USE_2X
	UCSR0A |= _BV(U2X0); // sets 2x baud rate to 1 (on)
	#else
	UCSR0A &= ~(_BV(U2X0)); // sets 2x baud rate to 0 (off)
	#endif
	*/
	
	UCSR0A &= ~(_BV(U2X0)); // sets 2x baud rate to 0 (off)

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
	
	stdout = &uart_str;
}

void UART_putc(char data)
{
	// wait for transmit buffer to be empty
	while(!(UCSR0A & _BV(UDRE0)));
	// load single byte into transmit register
	UDR0 = data;
	asm volatile("nop");
}

void UART_puts(char* s)
{
	// transmit byte until NULL is reached
	while(*s > 0) UART_putc(*s++);
}

