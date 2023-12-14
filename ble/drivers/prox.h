/*
 * prox.h
 *
 *  Created on: Nov 13, 2023
 *      Author: Cade Kenyon, and Kayla Thrane 
 */

#ifndef prox_H__
#define prox_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdio.h>

/* Macros */
#define PIN_MCU_PROX_GPIO P10_2 
#define PIN_MCU_PROX_EN P10_0
// update to go through io exapander

/* Public API */
cy_rslt_t prox_init(void);
uint8_t prox_read0(void);
void prox_shutdown(void);

#endif 