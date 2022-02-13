/*
 * UART.h
 *
 * Created: 9/10/2021 12:32:28 PM
 *  Author: Emin Eminof
 */ 

#ifndef UART_H_
#define UART_H_

#include "main.h"

void uart_init(void);
void UART_putc(char data);
void UART_puts(char* s);
FILE uart_str;

#endif /* UART_H_ */

