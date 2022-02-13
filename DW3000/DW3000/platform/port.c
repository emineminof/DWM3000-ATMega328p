/*
 * port.c
 *
 * Created: 9/10/2021 1:20:05 PM
 *  Author: Emim Eminof
 */ 

#include "port.h"

void sleepms(uint32_t x)
{
	_delay_ms(x); // delay by milliseconds
}

int sleepus(uint32_t x)
{
	_delay_us(x); // delay by microseconds
	return 0;
}

void deca_sleep(uint8_t time_ms) // wrapper for decawave sleep function
{
	sleepms(time_ms);
}

void deca_usleep(uint8_t time_us) // wrapper for decawave sleep function
{
	sleepus(time_us);
}


void spi_init(void)
{
	DDR_SPI = _BV(DD_MOSI)|_BV(DD_SCK)|_BV(DD_SS); // Set MOSI, SCK and CS output
	DDR_SPI &= ~_BV(DD_MISO); // make sure MISO is an input
	SPCR0 = _BV(SPE)|_BV(MSTR); // Enable SPI functionality and Master SPI mode
	SPCR0 &= ~_BV(DORD); // set SPI most significant bit first (this is default on ATMEGA328pb)
	
}

void open_spi(void)
{
	PORTB &= ~_BV(PORTB2); // set SS pin to LOW to enable SPI
}

void close_spi(void)
{
	PORTB |= _BV(PORTB2); // set SS pin to HIGH to disable SPI
}

int spi_tranceiver (uint8_t *data) // send single byte
{
	SPDR0 = data; // Load data into the buffer
	sleepus(1);
	while(!(SPSR0 & _BV(SPIF) )); // Wait until transmission complete
	
	// Return received data
	return(SPDR0);
}

int readfromspi(uint16_t headerLength, uint8_t *headerBuffer, uint16_t readLength, uint8_t *readBuffer)
{
	open_spi(); // we first open the SPI line by setting it to low
	for(int i=0; i<headerLength; i++) // write our header bytes to selected chip
	{
		spi_tranceiver(headerBuffer[i]);
	}
	for(int i=0; i<readLength; i++)
	{
		readBuffer[i] = spi_tranceiver(0x00); // store read byte value in the buffer
	
	}
	sleepus(5); // not sure if this is needed?
	close_spi(); // close the SPI line by setting it to high
	return 0;
}

int writetospi(uint16_t headerLength, uint8_t *headerBuffer, uint16_t bodyLength, uint8_t *bodyBuffer)
{
	open_spi(); // we first open the SPI line by setting it to low
	for(int i=0; i<headerLength; i++) // write our header bytes to selected chip
	{
		spi_tranceiver(headerBuffer[i]);	
		
	}
	for(int i=0; i<bodyLength; i++) // write our body data bytes to the selected chip
	{
		spi_tranceiver(bodyBuffer[i]);
		
	}
	sleepus(5); // not sure if this is needed?
	close_spi(); // close the SPI line by setting it to high
	return 0;
}

void port_set_dw_ic_spi_slowrate(void)
{
	SPSR0 &= ~_BV(SPI2X); // turn off fast speed
}

void port_set_dw_ic_spi_fastrate(void)
{
	SPSR0 |= _BV(SPI2X); // set fast speed by changing oscillator speed to FOSC / 2
}

void reset_DWIC(void) // currently not used as we are using softreset()
{
	DDR_PORTD |= _BV(DD_RESET_PIN); // set reset PIN as output
	PORTD &= ~_BV(PORTD7); // set reset pin to low for brief amount of time

	sleepus(1);

	DDR_PORTD &= ~_BV(DD_RESET_PIN); // set reset pin to input again

	sleepms(2); // allow for chip to turn back on

}

