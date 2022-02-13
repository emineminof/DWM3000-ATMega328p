/*
 * port.h
 *
 * Created: 9/10/2021 1:20:00 PM
 *  Author: Emin Eminof
 */ 

#ifndef PORT_H_
#define PORT_H_

#include "main.h"

#define DDR_SPI DDRB
#define DD_SCK DDB5
#define DD_MISO DDB4
#define DD_MOSI DDB3
#define DD_SS DDB2

#define DDR_PORTD DDRD
#define DD_RESET_PIN DDD7

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

void sleepms(uint32_t x);
int sleepus(uint32_t x);
void deca_sleep(uint8_t time_ms);
void deca_usleep(uint8_t time_us);
void spi_init(void);
void open_spi(void);
void close_spi(void);
int readfromspi(uint16_t headerLength, uint8_t *headerBuffer, uint16_t readLength, uint8_t *readBuffer);
int writetospi(uint16_t headerLength,  uint8_t *headerBuffer, uint16_t bodyLength, uint8_t *bodyBuffer);
void port_set_dw_ic_spi_slowrate(void);
void port_set_dw_ic_spi_fastrate(void);
void reset_DWIC(void);
int spi_tranceiver (uint8_t *data);

#endif /* PORT_H_ */

