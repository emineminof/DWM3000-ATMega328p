/*
 * main.h
 *
 * Created: 9/10/2021 1:33:09 PM
 *  Author: Emin Eminof
 */ 
#pragma once

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h> // all the standard AVR functions
#define __DELAY_BACKWARD_COMPATIBLE__ // this enables uint32 to be used in sleep functions
#include <util/delay.h>
#include <util/setbaud.h> // header for macros that calculate baud rates
#include <stdio.h>
#include <inttypes.h>
#include "UART.h"
#include "port.h"
#include "deca_device_api.h"
#include "shared_functions.h"

#define _BV(n) (1 << n) // sets 1 at position of BIT "n"
#define __INLINE inline

#endif /* MAIN_H_ */

