/*
 * simple_rx.c
 *
 * Created: 9/17/2021 4:45:01 PM
 *  Author: Emin Eminof
 */ 

#include "simple_rx.h"
#include "main.h"
#include "config_options.h"

int simple_rx(void)
{
	/* Default communication configuration. We use default non-STS DW mode. */
	static dwt_config_t config = {
		5,               /* Channel number. */
		DWT_PLEN_128,    /* Preamble length. Used in TX only. */
		DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
		9,               /* TX preamble code. Used in TX only. */
		9,               /* RX preamble code. Used in RX only. */
		1,               /* 0 to use standard 8 symbol SFD, 1 to use non-standard 8 symbol, 2 for non-standard 16 symbol SFD and 3 for 4z 8 symbol SDF type */
		DWT_BR_6M8,      /* Data rate. */
		DWT_PHRMODE_STD, /* PHY header mode. */
		DWT_PHRRATE_STD, /* PHY header rate. */
		(129 + 8 - 8),   /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
		DWT_STS_MODE_OFF, /* STS disabled */
		DWT_STS_LEN_64,  /* STS length see allowed values in Enum dwt_sts_lengths_e */
		DWT_PDOA_M0      /* PDOA mode off */
	};

	/* Buffer to store received frame. See NOTE 1 below. */
	static uint8_t rx_buffer[FRAME_LEN_MAX];
    /* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
    uint32_t status_reg;
    /* Hold copy of frame length of frame received (if good) so that it can be examined at a debug breakpoint. */
    uint16_t frame_len;	
	
	port_set_dw_ic_spi_slowrate();
	dwt_softreset(); // do a soft reset with SPI due to lack of RSTn line on our board
	
	sleepms(2); // Time needed for DW3000 to start up (transition from INIT_RC to IDLE_RC, or could wait for SPIRDY event)
	
	while (!dwt_checkidlerc()) // Need to make sure DW IC is in IDLE_RC before proceeding 
	{
		UART_puts("IDLE FAILED\r\n");
		return 0;
	}

	if (dwt_initialise(DWT_DW_INIT) == DWT_ERROR)
	{
		UART_puts("INIT FAILED\r\n");
		return 0;
	}
	
	// Enabling LEDs here for debug so that for each TX the D1 LED will flash on DW3000 red eval-shield boards.
	dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK);

	// Configure DW IC. See NOTE 5 below.
	if(dwt_configure(&config_options)) // if the dwt_configure returns DWT_ERROR either the PLL or RX calibration has failed the host should reset the device
	{
		UART_puts("CONFIG FAILED\r\n");
		return 0;
	}
	
	port_set_dw_ic_spi_fastrate();
	
	UART_puts("CONGRATS!!! Config and Init functions complete with no errors!\r\n");
	UART_puts("Starting main loop..\r\n");
		
	// Loop forever sending frames periodically.
    while (1)
    {
        /* TESTING BREAKPOINT LOCATION #1 */

        /* Clear local RX buffer to avoid having leftovers from previous receptions  This is not necessary but is included here to aid reading
         * the RX buffer.
         * This is a good place to put a breakpoint. Here (after first time through the loop) the local status register will be set for last event
         * and if a good receive has happened the data buffer will have the data in it, and frame_len will be set to the length of the RX frame. */
        memset(rx_buffer,0,sizeof(rx_buffer));

        /* Activate reception immediately. See NOTE 2 below. */
        dwt_rxenable(DWT_START_RX_IMMEDIATE);

        /* Poll until a frame is properly received or an error/timeout occurs. See NOTE 3 below.
         * STATUS register is 5 bytes long but, as the event we are looking at is in the first byte of the register, we can use this simplest API
         * function to access it. */
        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_ERR )))
        { };

        if (status_reg & SYS_STATUS_RXFCG_BIT_MASK)
        {
            /* A frame has been received, copy it to our local buffer. */
            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_BIT_MASK;
            if (frame_len <= FRAME_LEN_MAX)
            {
                dwt_readrxdata(rx_buffer, frame_len-FCS_LEN, 0); /* No need to read the FCS/CRC. */
            }

            /* Clear good RX frame event in the DW IC status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG_BIT_MASK);

            UART_puts("FRAME RECEIVED\r\n");

        }
        else
        {
            /* Clear RX error events in the DW IC status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
        }
    }
}

