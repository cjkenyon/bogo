/*
 * drive.c
 *
 *  Created on: Oct 22, 2023
 *      Author: Cade 
 */

#ifndef drive_H__
#define drive_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdio.h>

/* Macros */
#define PIN_MCU_DRIVE_A1 P9_4
#define PIN_MCU_DRIVE_A2 P9_6
#define PIN_MCU_DRIVE_B1 P10_4
#define PIN_MCU_DRIVE_B2 P10_6

/* Public Global Variables */
extern cyhal_pwm_t mDrive;
extern char bogo_dir;

/* Public API */
cy_rslt_t drive_init(void);
cy_rslt_t drive_dir(int direction);
#endif 


