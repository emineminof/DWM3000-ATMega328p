/*
 * main.c
 *
 * Created: 9/10/2021 12:21:11 PM
 *  Author: Emin Eminof
 */ 

#include "main.h"
#include "config_options.h"

#define SS_TWR_INITIATOR // the main function to be run

int main(void)
{	
	uart_init(); // setup UART and enable TX and RX
	sleepus(1); // give a microsecond so that things can print fine on serial
	spi_init(); // setup our SPI functions
	
	#ifdef SIMPLE_TX
		#include "simple_tx.h"
		simple_tx();
	#endif
	
	#ifdef SIMPLE_RX
		#include "simple_rx.h"
		simple_rx();
	#endif	
	
	#ifdef SS_TWR_INITIATOR
		#include "SS_TWR_INITIATOR.h"
		ds_twr_initiator();
	#endif
	
	#ifdef SS_TWR_RESPONDER
		#include "SS_TWR_RESPONDER.h"
		ds_twr_responder();
	#endif

}

