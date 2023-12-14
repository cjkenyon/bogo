/*
 * pump.h
 *
 *  Created on: Nov 13, 2023
 *      Author: Kayla Thrane 
 */

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "i2c.h"
#include "servo.h"

/**
 * Turn the pump on and off
*/
void pump(int on_off);
